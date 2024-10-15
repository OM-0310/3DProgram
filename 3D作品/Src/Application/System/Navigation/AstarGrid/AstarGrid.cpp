#include "AstarGrid.h"

#include "../AstarNode/AstarNode.h"

AstarGrid::AstarGrid(int _width,int _depth) : m_width(_width),m_depth(_depth)
{
	// グリッドを初期化
	for (int x = 0; x < m_width; x++)
	{
		std::vector<std::shared_ptr<AstarNode>> column;
		for (int z = 0; z < m_depth; z++)
		{
			column.push_back(std::make_shared<AstarNode>(x, z));
		}
		m_Grid.push_back(column);
	}
}

std::shared_ptr<AstarNode> AstarGrid::GetNode(int _x, int _z)
{
	if (_x >= 0 && _x < m_width && _z >= 0 && _z < m_depth)
	{
		return m_Grid[_x][_z];
	}
	return nullptr;
}

// 指定されたノードの隣接ノードを取得する。隣接ノードは、現在のノードの上下左右前後に位置する6方向のノード
std::vector<std::shared_ptr<AstarNode>> AstarGrid::GetNeighbors(std::shared_ptr<AstarNode> _currentNode)
{
	std::vector<std::shared_ptr<AstarNode>> neighbors;
	int x = _currentNode->GetPosX();
	int z = _currentNode->GetPosZ();
	int directions[8][2] = 
	{
		{ 1, 0 }, {-1,  0 }, { 0,  1 }, {  0,-1 },
		{ 1, 1 }, {-1, -1 }, { 1, -1 }, { -1, 1 } 
	};

	for (const auto& dir : directions)
	{
		std::shared_ptr<AstarNode>neighbor = GetNode(x + dir[0], z + dir[1]);
		if (neighbor && !neighbor->GetIsObstacle())
		{
			neighbors.push_back(neighbor);
		}
	}

	return neighbors;
}

void AstarGrid::SetObstacle(int _x, int _z, bool _isObstacle)
{
	auto node = GetNode(_x, _z);
	if (node)
	{
		node->SetIsObstacle(_isObstacle);
	}
}
