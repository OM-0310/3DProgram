#pragma once

class GameScene;

class Squeal :public KdGameObject
{
public:

	Squeal(){}
	~Squeal()override { Release(); }

	void Init()override;

	void Update()override;
	void PostUpdate()override;

	void DrawUnLit()override;

	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }
	void SetOwner(GameScene* _owner) { m_owner = _owner; }

private:

	void Release() { m_spPoly = nullptr; }

private:

	std::shared_ptr<KdSquarePolygon>	m_spPoly;

	Math::Matrix						m_mTrans;
	Math::Matrix						m_mScale;
	Math::Vector3						m_pos;
	Math::Vector3						m_size;

	float								m_hitArea;
	int									m_count;

	GameScene*							m_owner;
};
