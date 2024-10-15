#pragma once

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
// PathFindingクラスはA*アルゴリズムを使って経路探索を行う。
// このクラスは静的な関数を提供しAstarNodeおよびAstarGridクラスと連携してパスを計算する。
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

class AstarNode;
class AstarGrid;

class PathFinding
{
public:

	PathFinding	(){}
	~PathFinding(){}

	// 2つのノード間の※マンハッタン距離を計算するヒューステリック関数。
	// これはA*アルゴリズムでゴールまでの推定コストを計算するために使用される。
	static float Heuristic	
	(
		const std::shared_ptr<AstarNode>& _firstNode,
		const std::shared_ptr<AstarNode>& _secondNode
	);

	// ノードの比較関数(fCostが小さい方が優先)
	static bool CompareNodes(const std::shared_ptr<AstarNode>& _firstNode, const std::shared_ptr<AstarNode>& _secondNode);

	// A*アルゴリズムを使用して、指定された開始ノードからゴールノードまでの最短パスを計算する関数。
	static std::vector<std::shared_ptr<AstarNode>> Astar
	(
		std::shared_ptr<AstarNode> _startNode,
		std::shared_ptr<AstarNode> _goalNode,
		AstarGrid& _grid
	);
};

// ※ユークリテッド距離	 …	ユークリテッド距離とは、人が定規で測るような二点間「通常」の距離のことである。
// ※マンハッタン距離	 … マンハッタン距離とは、各座標の差(の絶対値)の総和を2点間の距離の事