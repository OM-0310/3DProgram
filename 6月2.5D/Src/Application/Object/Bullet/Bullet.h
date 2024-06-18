#pragma once

class GameScene;

class Bullet :public KdGameObject
{
public:

	Bullet(){}
	~Bullet()override { Release(); }

	void Update()override;
	void PostUpdate()override;

	void Init()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	void OnHit()override { m_isExpired = true; }

	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }
	void SetMove(Math::Vector3 _move) { m_move = _move; }

	void SetOwner(GameScene* _owner) { m_owner = _owner; }

private:

	void Release() { m_spPoly = nullptr; }

private:

	GameScene*							m_owner;

	std::shared_ptr<KdSquarePolygon>	m_spPoly;

	Math::Vector3						m_pos;
	Math::Vector3						m_move;
	Math::Vector3						m_size;

	Math::Matrix						m_mTrans;
	Math::Matrix						m_mScale;

	float								m_hitArea;
	float								m_sizeX;
};
