#include "PathFinding.h"

#include "../AstarNode/AstarNode.h"
#include "../AstarGrid/AstarGrid.h"

//================================================================================================================================
// 2つのノード間のマンハッタン距離を計算するヒューステリック関数・・・ここから
//================================================================================================================================
float PathFinding::Heuristic
(
	const std::shared_ptr<AstarNode>& _firstNode,
	const std::shared_ptr<AstarNode>& _secondNode
)
{
	// _firstNodeと_secondNodeのX座標とZ座標の絶対値の和を計算する。
	return static_cast<float>(std::abs(_firstNode->GetPosX() - _secondNode->GetPosX())) + static_cast<float>(std::abs(_firstNode->GetPosZ() - _secondNode->GetPosZ()));
}
//================================================================================================================================
// 2つのノード間のマンハッタン距離を計算するヒューステリック関数・・・ここまで
//================================================================================================================================

//================================================================================================================================
// ノードの比較関数(fCostが小さい方が優先)・・・ここから
//================================================================================================================================
bool PathFinding::CompareNodes(const std::shared_ptr<AstarNode>& _firstNode, const std::shared_ptr<AstarNode>& _secondNode)
{
	// 合計コストが大きい方を優先するようにして、std::priority_queueのデフォルトの動作(大きい値を優先)を逆転させる。
	return _firstNode->fCost() > _secondNode->fCost();
}
//================================================================================================================================
// ノードの比較関数(fCostが小さい方が優先)・・・ここまで
//================================================================================================================================

//================================================================================================================================
// A*アルゴリズムを使用して、指定された開始ノードからゴールノードまでの最短パスを計算する関数・・・ここから
//================================================================================================================================
std::vector<std::shared_ptr<AstarNode>> PathFinding::Astar
(
	std::shared_ptr<AstarNode> _startNode,
	std::shared_ptr<AstarNode> _goalNode,
	AstarGrid& _grid
)
{
	// ※priority_queueと※unordered_setを使用して効率化
	
	// ※std::priority_queue…C++で利用できるコンテナ・アダプタの一つで、優先順位付きキューを実現しているもの。
	//	 テンプレート・パラメータによって優先順位のつけ方を変更できる
	
	// ※std::unordered_set…集合を表現するコンテナで、各要素がユニーク(重複しない)であることが保証されている。
	//   内部ではハッシュテーブルを利用しており、要素の検索や追加、削除が平均的には定数で行えるのが特徴。
	//   大量のデータを扱う際に非常に有効な性質。

	// 探索中のノード(openList)と探索済みのノード(closedList)を用意
	std::priority_queue<std::shared_ptr<AstarNode>, std::vector<std::shared_ptr<AstarNode>>, decltype(&CompareNodes) >	openList;
	std::unordered_set<std::shared_ptr<AstarNode>>																		openSet;	// オープンリストの存在確認用
	std::unordered_set<std::shared_ptr<AstarNode>>																		closedList;

	openList.push(_startNode);
	openSet.insert(_startNode); // openListと同時にopenSetにも追加

	// openListが空でない限りループ
	while (!openList.empty())
	{
		// fCostが最小のノードを取得
		std::shared_ptr<AstarNode> currentNode = openList.top();
		openList.pop();
		openSet.erase(currentNode); // openSetから削除

		// もし現在のノードがゴールノードならば
		if (currentNode == _goalNode)
		{
			// 経路を再構築して返す
			std::vector<std::shared_ptr<AstarNode>> path;
			while (currentNode)
			{
				path.push_back(currentNode);
				currentNode = currentNode->GetParent().lock();
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		closedList.insert(currentNode);

		// 現在のノードの隣接ノードを取得
		std::vector<std::shared_ptr<AstarNode>> neighbors = _grid.GetNeighbors(currentNode);

		for (std::vector<std::shared_ptr<AstarNode>>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
		{
			std::shared_ptr<AstarNode>& neighbor = *it;

			// 探索済みのノードでないとき早期コンティニュー
			if (closedList.find(neighbor) != closedList.end())continue;

			// 斜め移動は√2のコスト、直線移動は1のコストとする
			float moveCost = (neighbor->GetPosX() != currentNode->GetPosX() && neighbor->GetPosZ() != currentNode->GetPosZ()) ? 1.414f : 1.0f;

			// 現在のノードから隣接ノードへのコストを加えた値
			float tetativeGCost = currentNode->GetGCost() + moveCost;

			// "openSet"を使って要素が存在するかを確認
			if (tetativeGCost < neighbor->GetGCost() || openSet.find(neighbor) == openSet.end())
			{
				neighbor->SetGCost(tetativeGCost);
				neighbor->SetHCost(Heuristic(neighbor, _goalNode));
				neighbor->SetParent(currentNode);

				// 隣接ノードがopenSetに存在しない場合
				if (openSet.find(neighbor) == openSet.end())
				{
					openList.push(neighbor);	// 要素を追加
					openSet.insert(neighbor);	// openSetにも追加
				}
			}
		}
	}

	// パスが見つからなかった場合は空のベクトルを返す
	return {};
}
//================================================================================================================================
// A*アルゴリズムを使用して、指定された開始ノードからゴールノードまでの最短パスを計算する関数・・・ここまで
//================================================================================================================================
