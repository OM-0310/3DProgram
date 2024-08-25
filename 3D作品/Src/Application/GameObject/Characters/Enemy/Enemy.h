#pragma once
#include "../CharaBase.h"

class Player;

class Enemy : public CharaBase
{
public:

	Enemy				()				{}
	~Enemy				()	override	{}

	void Init			()	override;
	void Update			()	override;
	void PostUpdate		()	override;

	void SearchProcess	();	// 索敵処理

	void SetPlayer		(std::shared_ptr<Player> _spPlayer) { m_wpPlayer = _spPlayer; }

	enum class StateType
	{
		Normal,				// 通常
		Wary,				// 警戒
		Discover			// 発見
	};

	enum class EnemyType
	{
		Idle,
	};

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Player>		m_wpPlayer;

	Math::Vector3				m_moveDir;
	float						m_moveSpeed;

	bool						m_waryFlg;
	bool						m_discoverFlg;
};
