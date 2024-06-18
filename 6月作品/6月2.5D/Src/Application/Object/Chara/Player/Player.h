#pragma once
#include "../CharaBase.h"

class Wild_Zombie;
class Zombie_Woman;
class Bullet;
class GameScene;

class Player:public CharaBase
{
public:

	Player();
	~Player()override;

	void Update()override;
	void PostUpdate()override;

	void IdleUpdate();
	void WalkUpdate();
	void RunUpdate();
	void AtkUpdate();
	void ShotUpdate();
	void ThrowUpdate();
	void HurtUpdate();
	void DeadUpdate();
	void ReloadUpdate();

	void Init()override;

	void GunShot();
	void GreThrow();
	void Melee();
	void UnableAct();

	void SetHPDec(int _decNum)override;
	void SetWild_Zombie(std::weak_ptr<Wild_Zombie> _wpw_Zombie);
	void SetZombie_Woman(std::weak_ptr<Zombie_Woman> _wpZombie_w);
	void SetOwner(GameScene* _owner) { m_owner = _owner; }

	std::weak_ptr<Bullet> GetBullet() { return m_wpBullet; }

	Math::Vector3 GetPos() { return m_pos; }
	int GetHP()override { return m_HP; }
	int GetBulletNum() { return m_bulletNum; }
	int GetGrenadeNum() { return m_grenadeNum; }
	static const int GetAtkNum() { return ATK_NUM; }
	float GetAtkAnim() { return m_atkAnim; }
	float GetSizeX() { return m_sizeX; }

	// デバッグ用
	void Death();

private:

	enum PlayerType
	{
		Idle,	// 0:直立
		Walk,	// 1:歩行
		Run,	// 2:走行
		Atk,	// 3:近接攻撃
		Shot,	// 4:銃発射
		Throw,	// 5:投擲
		Hurt,	// 6:ダメージ
		Dead,	// 7:死亡
		Reload	// 8:リロード
	};

	enum StatusType
	{
		Normal,	// 0:通常
		Apraxia	// 1:行動不能
	};

	static const int	IDLE_NUM	= 7;
	static const int	WALK_NUM	= 7;
	static const int	RUN_NUM		= 8;
	static const int	ATK_NUM		= 3;
	static const int	SHOT_NUM	= 4;
	static const int	THROW_NUM	= 9;
	static const int	HURT_NUM	= 3;
	static const int	DEAD_NUM	= 4;
	static const int	RELOAD_NUM	= 13;

	static const int	BULLET_MAX	= 31;
	static const int	GRENADE_MAX = 5;

	void Release();

private:

	GameScene*					m_owner;

	std::weak_ptr<Wild_Zombie>	m_wpw_Zombie;
	std::weak_ptr<Zombie_Woman> m_wpZombie_w;
	std::weak_ptr<Bullet>		m_wpBullet;

	PlayerType					m_pType;
	StatusType					m_statysType;

	int							m_idleCnt[IDLE_NUM]		= { 0,1,2,3,4,5,6 };
	int							m_walkCnt[WALK_NUM]		= { 7,8,9,10,11,12,13 };
	int							m_runCnt[RUN_NUM]		= { 14,15,16,17,18,19,20,21 };
	int							m_atkCnt[ATK_NUM]		= { 25,26,27 };
	int							m_shotCnt[SHOT_NUM]		= { 32,33,34,35 };
	int							m_throwCnt[THROW_NUM]	= { 36,37,38,39,40,41,42,43,44 };
	int							m_hurtCnt[HURT_NUM]		= { 22,23,24 };
	int							m_deadCnt[DEAD_NUM]		= { 28,29,30,31 };
	int							m_reloadCnt[RELOAD_NUM] = { 45,46,47,48,49,50,51,52,53,54,55,56,57 };
	int							m_shotWait;
	int							m_throwWait;
	int							m_apraxiaWait;

	float						m_idleAnim;
	float						m_walkAnim;
	float						m_runAnim;
	float						m_atkAnim;
	float						m_shotAnim;
	float						m_throwAnim;
	float						m_hurtAnim;
	float						m_deadAnim;
	float						m_reloadAnim;

	bool						m_atkFlg;
	bool						m_shotFlg;
	bool						m_setFlg;
	bool						m_greFlg;
	bool						m_relFlg;
	bool						m_hurtFlg;
	bool						m_deadFlg;
	bool						m_keyFlg;

	int							m_bulletNum;
	int							m_grenadeNum;
};
