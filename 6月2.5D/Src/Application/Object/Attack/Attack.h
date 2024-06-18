#pragma once

class GameScene;

class Attack :public KdGameObject
{
public:

	Attack(){}
	~Attack()override { Release(); }

	void Init()override;
	void Update()override;

	void OnHit()override { m_isExpired = true; }

	void SetPos(Math::Vector3 _pos) { m_pos = _pos; }
	void SetOwner(GameScene* _owner) { m_owner = _owner; }

private:

	void Release(){}

private:

	Math::Matrix	m_mTrans;
	Math::Vector3	m_pos;

	float			m_attackArea;
	int				m_attackCnt;
	int				m_attackCntMax;

	GameScene*		m_owner;
};
