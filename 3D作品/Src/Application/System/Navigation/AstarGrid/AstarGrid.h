#pragma once

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
// AstarGridクラスは3D空間におけるグリッド全体を管理する。
// 各ノードの配置と障害物の設定、隣接ノードの取得などの機能を提供する。
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

class AstarNode;

class AstarGrid
{
public:

	AstarGrid	(int _width, int _depth);
	~AstarGrid	()			{}

	std::shared_ptr<AstarNode>				GetNode		(int _x, int _z);
	std::vector<std::shared_ptr<AstarNode>> GetNeighbors(std::shared_ptr<AstarNode> _currentNode);

	void									SetObstacle(int _x, int _z, bool _isObstacle);
	
private:

	int m_width;													// グリッドの幅
	int m_depth;													// グリッドの奥行き
	std::vector<std::vector<std::shared_ptr<AstarNode>>> m_Grid;	// グリッド構成するノードの2次元配列	
};
