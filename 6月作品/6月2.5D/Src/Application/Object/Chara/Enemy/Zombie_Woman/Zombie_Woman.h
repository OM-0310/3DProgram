#pragma once
#include "../../CharaBase.h"

class Player;
class GameScene;

class Zombie_Woman :public CharaBase
{
public:

	Zombie_Woman();
	~Zombie_Woman()override;

	void Update()override;
	void PostUpdate()override;

	void IdleUpdate();
	void WalkUpdate();
	void ScreamUpdate();
	void HurtUpdate();
	void DeadUpdate();

	void DrawLit()override;

	void Init()override;

	void Death()override { m_isExpired = true; }
	void Follow();
	void Loudly();
	void KnockBuck()override;

	void SetHPDec(int _decNum)override;
	void SetPlayer(std::weak_ptr<Player> _wpPlayer) { m_wpPlayer = _wpPlayer; }
	void SetOwner(GameScene* _owner) { m_owner = _owner; }

	int GetHP()override { return m_HP; }
	std::shared_ptr<Player> GetPlayer();

private:

	enum Zombie_WomanType
	{
		Idle,	// 0:直立時
		Walk,	// 1:歩行時
		Scream,	// 2:叫び時
		Hurt,	// 3:怯み時
		Dead,	// 4:死亡時
	};

	static const int IDLE_NUM	= 5;
	static const int WALK_NUM	= 7;
	static const int SCREAM_NUM = 5;
	static const int HURT_NUM	= 3;
	static const int DEAD_NUM	= 5;

	void Release();

private:

	std::weak_ptr<Player> m_wpPlayer;

	Zombie_WomanType					m_zwType;
	int									m_idleCnt[IDLE_NUM]		= { 4,5,6,7,8 };
	int									m_walkCnt[WALK_NUM]		= { 9,10,11,12,13,14,15 };
	int									m_screamCnt[SCREAM_NUM] = { 31,32,33,34,35 };
	int									m_hurtCnt[HURT_NUM]		= { 23,24,25 };
	int									m_deadCnt[DEAD_NUM]		= { 26,27,28,29,30 };

	int									m_actionCnt;
	int									m_screamActCnt;
	int									m_screamWait;
	float								m_dissolveCnt;
	float								m_searchArea;
	float								m_screamArea;
	bool								m_followFlg;

	float								m_idleAnim;
	float								m_walkAnim;
	float								m_screamAnim;
	float								m_hurtAnim;
	float								m_deadAnim;

	bool								m_idleFlg;
	bool								m_walkFlg;
	bool								m_screamFlg;
	bool								m_hurtFlg;
	bool								m_deadFlg;

	GameScene*							m_owner;
};
