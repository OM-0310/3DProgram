#pragma once

//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
// AstarNodeクラスは、グリッド内の各ノード(セル)を表す。
// このクラスは、各ノードの位置、コスト、障害物の情報を保持している。 
//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

class AstarNode
{
public:
	
	AstarNode							(int _posX, int _posZ); // コンストラクタ
	~AstarNode							()				{}

	// ノードの合計コストを返す関数
	float fCost							()	 const;

	// ノードの透過性を確認するための演算子オーバーロード
	bool operator==	(const AstarNode& _other) const;

	// ゲッター
	int GetPosX							()	const		{ return m_posX; }
	int GetPosZ							()	const		{ return m_posZ; }
	float GetGCost						()	const		{ return m_gCost; }
	float GetHCost						()	const		{ return m_hCost; }
	std::weak_ptr<AstarNode> GetParent	()	const		{ return m_wpParent; }
	bool GetIsObstacle					()	const		{ return m_isObstacle; }

	// セッター
	void SetPosX(int _posX)								{ m_posX = _posX; }
	void SetPosZ(int _posZ)								{ m_posZ = _posZ; }
	void SetGCost(float _gCost)							{ m_gCost = _gCost; }
	void SetHCost(float _hCost)							{ m_hCost = _hCost; }
	void SetParent(std::shared_ptr<AstarNode> _spParent){ m_wpParent = _spParent; }
	void SetIsObstacle(bool _isObstacle)				{ m_isObstacle = _isObstacle; }


private:

	int						m_posX;			// ノード座標X
	int						m_posZ;			// ノード座標Z
	float					m_gCost;		// 開始ノードからこのノードまでの実際のコスト(距離や移動コスト)
	float					m_hCost;		// このノードから目標ノードまでの推定コスト(ヒューステリック値)
	std::weak_ptr<AstarNode>m_wpParent;		// 経路を追跡するための親ノード
	bool					m_isObstacle;	// 障害物であるかどうかを示すフラグ

};
