#pragma once
#include "../CharaBase.h"

class Enemy_Main;
class Enemy_Gun;
class Enemy_Gun_NoMagazine;
class Enemy_Magazine;
class Player;
class AstarNode;
class AstarGrid;

class Enemy : public CharaBase
{
public:

	Enemy				()				{}
	~Enemy				()	override	{}

	void Init			()	override;
	void Update			()	override;
	void PostUpdate		()	override;

	void UpdateCollision();

	void SearchProc		();						// 索敵処理
	void ChaseProc		();						// 追跡処理
	void LostProc		();						// 失踪処理
	void ChangeAressted	();						// 拘束状態切り替え処理
	void ChangeIdle		();						// 停止状態切り替え処理
	void Death			(const bool _deathFlg); // 死亡処理

	void ChangeAnimation(const std::string& _animeName, bool _isLoop = true, float _time = 0.0f);	// アニメーション切り替え処理置に移動させる処理

	void SetEnemy_Main			(const std::shared_ptr<Enemy_Main>& _spEnemy_Main) 
	{ m_wpEnemy_Main = _spEnemy_Main; }
	void SetEnemy_Gun			(const std::shared_ptr<Enemy_Gun>& _spEnemy_Gun) 
	{ m_wpEnemy_Gun = _spEnemy_Gun; }
	void SetEnemy_Gun_NoMagazine(const std::shared_ptr<Enemy_Gun_NoMagazine>& _spEnemy_Gun_NoMgazine) 
	{ m_wpEnemy_Gun_NoMagazine = _spEnemy_Gun_NoMgazine; }
	void SetEnemy_Magazine			(const std::shared_ptr<Enemy_Magazine>& _spEnemy_Magazine) 
	{ m_wpEnemy_Magazine = _spEnemy_Magazine; }
	void SetPlayer				(const std::shared_ptr<Player>& _spPlayer) 
	{ m_wpPlayer = _spPlayer; }

	const bool GetDissolveFlg() { return m_dissolveFlg; }

	enum class SituationType
	{
		Idle	= 1 << 0,	// 停止状態
		Walk	= 1 << 1,	// 歩行状態
		Discover= 1 << 2,	// 発見状態
		Run		= 1 << 3,	// 走行状態
		Ready	= 1 << 4,	// 構え状態
		Shot	= 1 << 5,	// 発射状態
		Aressted= 1 << 6,	// 拘束状態
		Reload	= 1 << 7,	// リロード状態
		Death	= 1 << 8	// 死亡状態
	};

private:

	std::shared_ptr<KdAnimator>			m_spAnimator;					// アニメーション管理系

	std::weak_ptr<Enemy_Main>			m_wpEnemy_Main;					// 敵(本体)情報
	std::weak_ptr<Enemy_Gun>			m_wpEnemy_Gun;					// 敵(銃)情報
	std::weak_ptr<Enemy_Gun_NoMagazine>	m_wpEnemy_Gun_NoMagazine;		// 敵(銃マガジン無し)情報
	std::weak_ptr<Enemy_Magazine>		m_wpEnemy_Magazine;				// 敵(マガジン)情報
	std::weak_ptr<Player>				m_wpPlayer;						// プレイヤー情報

	Math::Matrix		m_mMuzzle		= Math::Matrix::Identity;	// 銃口行列
	const Math::Vector3	m_muzzlePos		= { 0.19f,1.46f,1.0 };		// 銃口座標

	Math::Matrix		m_mlocalAresst	= Math::Matrix::Identity;
	const Math::Vector3 m_localAresstPos= { -0.1f,0.0f,0.25f };

	float			m_nowAnimeFrm	= 0.0f;						// 現在のアニメーションフレーム
	const float		m_zeroAnimFrm	= 0.0f;						// アニメーション初期化用変数
	const float		m_dscovrAnimFrm = 36.0f;					// 発見アニメーションフレーム 36.0f
	const float		m_shotAnimFrm	= 4.0f;						// 弾発射アニメーションフレーム 4.0f
	const float		m_reloadAnimFrm = 90.0f;					// リロードアニメーションフレーム 90.0f
	const float		m_deathAnimFrm	= 52.0f;					// 死亡アニメーションフレーム
	const float		m_animFrmSpd	= 1.0f;						// アニメーションの1フレーム

	float			m_dissolveCnt	= 0.0f;						// ディゾルブカウント
	const float		m_dissolveMax	= 1.0f;						// ディゾルブカウントのMAX値
	const float		m_dissolveSpd	= 0.1f;						// ディゾルブ速度
	bool			m_dissolveFlg	= false;					// ディゾルブフラグ

	Math::Vector3	m_moveDir		= Math::Vector3::Zero;		// 移動方向
	const float		m_arrivalZPos	= 27.0;						// 目的地Z座標
	float			m_moveSpeed		= 0.0f;						// 移動速度

	const float		m_zeroMoveSpd	= 0.0f;						// 停止時移動速度 0.0f
	const float		m_walkMoveSpd	= 0.03f;					// 歩行時移動速度 0.03f
	const float		m_runMoveSpd	= 0.07f;					// 走行時移動速度 0.07f

	const float		m_viewRange		= 0.1f;						// 視野範囲
	const float		m_waryArea		= 12.0f;					// 警戒範囲
	const float		m_discoverArea	= 11.0f;						// 発見範囲
	const float		m_shotArea		= 10.0f;						// 弾発射範囲

	bool			m_dirFlg		= false;					// 方向フラグ
	bool			m_waryFlg		= false;					// 警戒フラグ
	bool			m_findFlg		= false;					// 発見フラグ
	bool			m_deathFlg		= false;					// 死亡フラグ

	UINT			m_actionCnt		= 0;						// アクションカウント 0以上の整数のみを使用するためunsigned型で使用

	const int		m_maxShotWait	= 4;						// 発射待機時間MAX値
	int				m_shotWait		= 0;						// 発射待機時間

	bool			m_lostFlg		= false;					// 失踪フラグ
	UINT			m_lostCnt		= 0;						// 失踪カウント 0以上の整数のみを使用するためunsigned型で使用
	const UINT		m_lostCntMax	= 120;						// 失踪カウント最大値
	const UINT		m_lostCntMin	= 0;						// 失踪カウント最小値

	SituationType	m_sitType		= SituationType::Idle;

private:

	class ActionStateBase
	{
	public:

		virtual ~ActionStateBase() {}

		virtual void Enter	(Enemy& _owner) { (void)_owner; }
		virtual void Update	(Enemy& _owner) { (void)_owner; }
		virtual void Exit	(Enemy& _owner) { (void)_owner; }
	};

	class ActionIdle : public ActionStateBase
	{
	public:

		ActionIdle	()				{}
		~ActionIdle	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	class ActionWalk : public ActionStateBase
	{
	public:

		ActionWalk	()				{}
		~ActionWalk	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	class ActionDiscover : public ActionStateBase
	{
	public:

		ActionDiscover	()				{}
		~ActionDiscover	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	class ActionRun : public ActionStateBase
	{
	public:

		ActionRun	()				{}
		~ActionRun	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update(Enemy& _owner)	override;
	};

	class ActionReady : public ActionStateBase
	{
	public:

		ActionReady	()				{}
		~ActionReady()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	class ActionShot : public ActionStateBase
	{
	public:

		ActionShot	()				{}
		~ActionShot	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	class ActionAressted : public ActionStateBase
	{
	public:

		ActionAressted	()				{}
		~ActionAressted	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	class ActionReload : public ActionStateBase
	{
	public:

		ActionReload	()				{}
		~ActionReload	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	class ActionDeath : public ActionStateBase
	{
	public:

		ActionDeath		()				{}
		~ActionDeath	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> _nextState);
	std::shared_ptr<ActionStateBase> m_nowState = nullptr;
};
