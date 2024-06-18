#pragma once

class GameScene;

class Grenade :public KdGameObject
{
public:

	Grenade(){}
	~Grenade()override { Release(); }

	void Update()override;
	void PostUpdate()override;

	void Init()override;

	void Explosion() { m_isExpired = true; }

	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }
	void SetOwner(GameScene* _owner) { m_owner = _owner; }

private:

	void Release() { m_spPoly = nullptr; }

	static const int EXP_NUM = 9;

private:

	std::shared_ptr<KdSquarePolygon>	m_spPoly;

	Math::Vector3						m_pos;
	Math::Vector3						m_move;
	Math::Vector3						m_size;

	Math::Matrix						m_mTrans;
	Math::Matrix						m_mScale;

	const float							m_throwPow = 0.1f;
	float								m_gravity;
	float								m_sizeX;
	float								m_sizeY;
	float								m_expArea;
	int									m_anime[EXP_NUM] = { 0,1,2,3,4,5,6,7,8 };
	float								m_animSpeed;
	float								m_animCnt;
	bool								m_throwFlg;
	bool								m_animFlg;

	GameScene*							m_owner;
};
