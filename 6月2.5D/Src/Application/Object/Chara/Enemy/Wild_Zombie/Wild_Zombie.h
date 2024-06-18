#pragma once
#include "../../CharaBase.h"

class Player;

class Wild_Zombie :public CharaBase
{
public:
	Wild_Zombie();
	~Wild_Zombie()override;

	void Update()override;
	void PostUpdate()override;

	void IdleUpdate();
	void WalkUpdate();
	void RunUpdate();
	void JumpUpdate();
	void AtkUpdate();
	void HurtUpdate();
	void DeadUpdate();
	void EatUpdate();

	void DrawLit()override;

	void Init()override;

	void Death()override { m_isExpired = true; }
	void Follow();
	void Attack(bool _hit);
	void KnockBuck()override;

	void SetHPDec(int _decNum)override;
	void SetPlayer(std::weak_ptr<Player> _wpPlayer) { m_wpPlayer = _wpPlayer; }

	int GetHP()override { return m_HP; }
	std::shared_ptr<Player>GetPlayer();

	// デバッグ用
	int GetActionCnt() { return m_actionCnt; }

private:

	enum Wild_ZombieType
	{
		Idle,
		Walk,
		Run,
		Jump,
		Atk,
		Hurt,
		Dead,
		Eat
	};

	static const int IDLE_NUM	= 9;
	static const int ATK_NUM	= 12;
	static const int WALK_NUM	= 10;
	static const int RUN_NUM	= 8;
	static const int JUMP_NUM	= 6;
	static const int HURT_NUM	= 5;
	static const int DEAD_NUM	= 5;
	static const int EAT_NUM	= 11;

	void Release();

private:

	std::weak_ptr<Player> m_wpPlayer;

	Wild_ZombieType m_wzType;
	int		m_idleCnt[IDLE_NUM] = { 12,13,14,15,16,17,18,19,20 };
	int		m_walkCnt[WALK_NUM] = { 21,22,23,24,25,26,27,28,29,30 };
	int		m_runCnt[RUN_NUM]	= { 31,32,33,34,35,36,37,38 };
	int		m_jumpCnt[JUMP_NUM] = { 39,40,41,42,43,44 };
	int		m_atkCnt[ATK_NUM]	= { 0,1,2,3,4,5,6,7,8,9,10,11 };
	int		m_hurtCnt[HURT_NUM] = { 45,46,47,48,49 };
	int		m_deadCnt[DEAD_NUM] = { 50,51,52,53,54 };
	int		m_eatCnt[EAT_NUM]	= { 55,56,57,58,59,60,61,62,63,64,65 };

	float	m_idleAnim;
	float	m_walkAnim;
	float	m_runAnim;
	float	m_jumpAnim;
	float	m_atkAnim;
	float	m_hurtAnim;
	float	m_deadAnim;
	float	m_eatAnim;
	float	m_dissolveCnt;
	float	m_searchArea;
	float	m_attackArea;

	int		m_actionCnt;

	bool	m_idleFlg;
	bool	m_walkFlg;
	bool	m_atkFlg;
	bool	m_runFlg;
	bool	m_hurtFlg;
	bool	m_deadFlg;
	bool	m_eatFlg;

public:

	// シングルトンパターン
	// 常に存在する && 必ず1つしか存在しない(1つしか存在出来ない)
	// どこからでもアクセスが可能で便利だが
	// 何でもかんでもシングルトンという思考はNG
	static Wild_Zombie& Instance()
	{
		static Wild_Zombie instance;
		return instance;
	}
};
