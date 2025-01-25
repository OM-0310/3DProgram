#pragma once

#include "../UIBase.h"

class Player;
class Enemy_1;
class Enemy_2;
class Enemy_3;

class RestraintUI : public UIBase
{
public:

	RestraintUI		()				{}
	~RestraintUI	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetRestraintFlg(const bool _restraintFlg);

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	void SetEnemy_1(const std::shared_ptr<Enemy_1>& _spEnemy_1)
	{
		m_wpEnemy_1 = _spEnemy_1;
	}

	void SetEnemy_2(const std::shared_ptr<Enemy_2>& _spEnemy_2)
	{
		m_wpEnemy_2 = _spEnemy_2;
	}

	void SetEnemy_3(const std::shared_ptr<Enemy_3>& _spEnemy_3)
	{
		m_wpEnemy_3 = _spEnemy_3;
	}

private:

	std::weak_ptr<Player>	m_wpPlayer;
	std::weak_ptr<Enemy_1>	m_wpEnemy_1;
	std::weak_ptr<Enemy_2>	m_wpEnemy_2;
	std::weak_ptr<Enemy_3>	m_wpEnemy_3;

	bool					m_restraintFlg	= false;
	bool					m_able1Flg		= false;
	bool					m_able2Flg		= false;
	bool					m_able3Flg		= false;

	const short				m_lifeSpanMax	= 35;
	const short				m_lifeSpanMin	= 0;
	short					m_lifeSpan		= 0;
};
