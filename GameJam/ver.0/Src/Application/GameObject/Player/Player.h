#pragma once

class GameScene;
class MeetBlock;
class VegetableBlock;

class Player : public KdGameObject
{
public:

	Player					()													{}
	~Player					()	override										{}

	void Init				()	override;
	void Update				()	override;
	void DrawLit			()	override;

	//==食材================================================================

	void MoveProcess		();	// 移動処理
	void MeetGetProcess		();	// 食材取得処理(肉)
	void VegetaGetProcess	();	// 食材取得処理(野菜)


	void SetMeetBlock		(std::shared_ptr<MeetBlock> _spMeetBlock)			{ m_wpMeetBlock = _spMeetBlock; }
	void SetVegetableBlock	(std::shared_ptr<VegetableBlock> _spVegetaBlock)	{ m_wpVagetaBlock = _spVegetaBlock; }
	void SetOwner			(GameScene* _omner)									{ m_owner = _omner; }

	//======================================================================

private:

	//==食材================================================================

	std::weak_ptr<MeetBlock>			m_wpMeetBlock;		// 肉情報
	std::weak_ptr<VegetableBlock>		m_wpVagetaBlock;	// 野菜情報

	//======================================================================

	std::shared_ptr<KdSquarePolygon>	m_spPoly;

	Math::Vector3						m_pos;				// 座標
	Math::Vector3						m_moveDir;			// 方向ベクトル

	float								m_moveSpeed;		// 移動速度

	//==食材================================================================

	float								m_getArea;			// 食材取得エリア

	bool								m_keyFlg;			// キーフラグ
	bool								m_getFlg;			// 食材取得フラグ

	GameScene*							m_owner;

	//======================================================================
};
