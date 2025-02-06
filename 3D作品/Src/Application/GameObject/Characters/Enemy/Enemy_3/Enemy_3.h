#pragma once
#include "../../CharaBase.h"

class Enemy_Main_3;
class Enemy_Gun_3;
class Enemy_Gun_NoMagazine_3;
class Enemy_Magazine_3;
class Player;
class AstarNode;
class AstarGrid;

class Enemy_3 : public CharaBase
{
public:

	enum class SituationType
	{
		Idle			,	// 停止状態
		Walk			,	// 歩行状態
		Discover		,	// 発見状態
		Run				,	// 走行状態
		Ready			,	// 構え状態
		Shot			,	// 発射状態
		Aressted		,	// 拘束状態
		Aressted_Death	,	// 拘束状態
		Reload			,	// リロード状態
		Death				// 死亡状態
	};

	Enemy_3				()				{}
	~Enemy_3			()	override	{}

	void Init			()	override;
	void Update			()	override;
	void PostUpdate		()	override;

	void UpdateCollision();

	void SearchProc		();						// 索敵処理
	void ChaseProc		();						// 追跡処理
	void LostProc		();						// 失踪処理
	void ChangeAressted	();						// 拘束状態切り替え処理
	void Execution		();						// 処刑処理
	void ChangeIdle		();						// 停止状態切り替え処理
	void SetDeathFlg	(const bool _deathFlg); // 死亡処理
	void Answer			() { m_bitsEachFlg[AnswerFlg] = true; }

	void ChangeAnimation(const std::string& _animeName, bool _isLoop = true, float _time = 0.0f);	// アニメーション切り替え処理置に移動させる処理

	void SetPos(const Math::Vector3& _pos) { m_pos = _pos; }

	void SetEnemy_Main			
	(const std::shared_ptr<Enemy_Main_3>& _spEnemy_Main) { m_wpEnemy_Main = _spEnemy_Main; }

	void SetEnemy_Gun			
	(const std::shared_ptr<Enemy_Gun_3>& _spEnemy_Gun) { m_wpEnemy_Gun = _spEnemy_Gun; }

	void SetEnemy_Gun_NoMagazine
	(const std::shared_ptr<Enemy_Gun_NoMagazine_3>& _spEnemy_Gun_NoMgazine) { m_wpEnemy_Gun_NoMagazine = _spEnemy_Gun_NoMgazine; }

	void SetEnemy_Magazine			
	(const std::shared_ptr<Enemy_Magazine_3>& _spEnemy_Magazine) { m_wpEnemy_Magazine = _spEnemy_Magazine; }

	void SetPlayer				
	(const std::shared_ptr<Player>& _spPlayer) { m_wpPlayer = _spPlayer; }

	const bool GetDissolveFlg		()	const	{ return m_bitsEachFlg.test(DissolveFlg);	}
	const bool GetAnswerFlg			()	const	{ return m_bitsEachFlg.test(AnswerFlg);		}
	const SituationType& GetSitType	()	const	{ return m_sitType;							}


private:

	enum
	{
		AnswerFlg,	// 解答フラグ
		DeathFlg,	// 死亡フラグ
		DirFlg,		// 方向切り替えフラグ
		DissolveFlg,// ディゾルブフラグ
		FindFlg,	// 発見フラグ
		LostFlg,	// 失踪フラグ
	};

private:

	std::shared_ptr<KdAnimator>				m_spAnimator;					// アニメーション管理系

	std::weak_ptr<Enemy_Main_3>				m_wpEnemy_Main;					// 敵(本体)情報
	std::weak_ptr<Enemy_Gun_3>				m_wpEnemy_Gun;					// 敵(銃)情報
	std::weak_ptr<Enemy_Gun_NoMagazine_3>	m_wpEnemy_Gun_NoMagazine;		// 敵(銃マガジン無し)情報
	std::weak_ptr<Enemy_Magazine_3>			m_wpEnemy_Magazine;				// 敵(マガジン)情報
	std::weak_ptr<Player>					m_wpPlayer;						// プレイヤー情報

	Math::Matrix					m_mMuzzle			= Math::Matrix::Identity;	// 銃口行列
	static constexpr Math::Vector3	m_muzzlePos			= { 0.19f,1.46f,1.0 };		// 銃口座標

	Math::Matrix					m_mlocalAresst		= Math::Matrix::Identity;
	static constexpr Math::Vector3	m_localAresstPos	= { -0.1f,0.0f,0.25f };

	float						m_nowAnimeFrm	= 0.0f;						// 現在のアニメーションフレーム
	static constexpr float		m_zeroAnimFrm	= 0.0f;						// アニメーション初期化用変数
	static constexpr float		m_dscovrAnimFrm = 36.0f;					// 発見アニメーションフレーム 36.0f
	static constexpr float		m_shotAnimFrm	= 4.0f;						// 弾発射アニメーションフレーム 4.0f
	static constexpr float		m_reloadAnimFrm = 90.0f;					// リロードアニメーションフレーム 90.0f
	static constexpr float		m_exeAnimFrm	= 40.0f;					// 処刑アニメーションフレーム 40.0f
	static constexpr float		m_downAnimFrm	= 30.0f;					// 転倒アニメーションフレーム 30.0f
	static constexpr float		m_deathAnimFrm	= 52.0f;					// 死亡アニメーションフレーム 52.0f
	static constexpr float		m_animFrmSpd	= 1.0f;						// アニメーションの1フレーム

	float						m_dissolveCnt	= 0.0f;						// ディゾルブカウント
	static constexpr float		m_dissolveMax	= 1.0f;						// ディゾルブカウントのMAX値
	static constexpr float		m_dissolveSpd	= 0.1f;						// ディゾルブ速度

	Math::Vector3				m_moveDir		= Math::Vector3::Zero;		// 移動方向
	static constexpr float		m_arrivalZPos	= 27.0;						// 目的地Z座標
	float						m_moveSpeed		= 0.0f;						// 移動速度

	static constexpr float		m_zeroMoveSpd	= 0.0f;						// 停止時移動速度 0.0f
	static constexpr float		m_walkMoveSpd	= 0.03f;					// 歩行時移動速度 0.03f
	static constexpr float		m_runMoveSpd	= 0.07f;					// 走行時移動速度 0.07f

	static constexpr float		m_viewRange		= 0.9f;						// 視野範囲
	static constexpr float		m_waryArea		= 12.0f;					// 警戒範囲
	static constexpr float		m_discoverArea	= 11.0f;					// 発見範囲
	static constexpr float		m_shotArea		= 10.0f;					// 弾発射範囲

	static constexpr short		m_totalEachFlg = 6;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;

	short						m_actionCnt		= 0;						// アクションカウント 0以上の整数のみを使用するためunsigned型で使用

	static constexpr short		m_maxShotWait	= 4;						// 発射待機時間MAX値
	short						m_shotWait		= 0;						// 発射待機時間

	short						m_lostCnt		= 0;						// 失踪カウント 0以上の整数のみを使用するためunsigned型で使用
	static constexpr short		m_lostCntMax	= 120;						// 失踪カウント最大値
	static constexpr short		m_lostCntMin	= 0;						// 失踪カウント最小値

	SituationType				m_sitType		= SituationType::Idle;

	static constexpr float				m_runVol		= 0.5f;
	std::shared_ptr<KdSoundInstance3D>	m_spRunSound;

	static constexpr float				m_walkVol		= 0.1f;
	std::shared_ptr<KdSoundInstance3D>	m_spWalkSound;

	static constexpr float				m_readyVol		= 0.5f;
	std::shared_ptr<KdSoundInstance3D>	m_spReadySound;

	static constexpr float				m_shotVol		= 0.8f;
	std::shared_ptr<KdSoundInstance3D>	m_spShotSound;

	static constexpr float				m_mgznSetVol	= 0.7f;
	std::shared_ptr<KdSoundInstance3D>	m_spMgznSetSound;

	static constexpr float				m_mgznRlsVol	= 0.7f;
	std::shared_ptr<KdSoundInstance3D>	m_spMgznRlsSound;

	static constexpr float				m_discoverVol	= 0.5f;
	std::shared_ptr<KdSoundInstance>	m_spDiscoverSound;

	static constexpr float				m_deathVol		= 0.5f;
	std::shared_ptr<KdSoundInstance3D>	m_spDeathSound;

private:

	class ActionStateBase
	{
	public:

		virtual ~ActionStateBase() {}

		virtual void Enter	(Enemy_3& _owner) { (void)_owner; }
		virtual void Update	(Enemy_3& _owner) { (void)_owner; }
		virtual void Exit	(Enemy_3& _owner) { (void)_owner; }
	};

	class ActionIdle : public ActionStateBase
	{
	public:

		ActionIdle	()				{}
		~ActionIdle	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
	};

	class ActionWalk : public ActionStateBase
	{
	public:

		ActionWalk	()				{}
		~ActionWalk	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
		void Exit	(Enemy_3& _owner)	override;
	};

	class ActionDiscover : public ActionStateBase
	{
	public:

		ActionDiscover	()				{}
		~ActionDiscover	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
	};

	class ActionRun : public ActionStateBase
	{
	public:

		ActionRun	()				{}
		~ActionRun	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
		void Exit	(Enemy_3& _owner)	override;
	};

	class ActionReady : public ActionStateBase
	{
	public:

		ActionReady	()				{}
		~ActionReady()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
	};

	class ActionShot : public ActionStateBase
	{
	public:

		ActionShot	()				{}
		~ActionShot	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
		void Exit	(Enemy_3& _owner)	override;
	};

	class ActionAressted : public ActionStateBase
	{
	public:

		ActionAressted	()				{}
		~ActionAressted	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
	};

	class ActionAressted_Death : public ActionStateBase
	{
	public:

		ActionAressted_Death	()				{}
		~ActionAressted_Death	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
	};

	class ActionReload : public ActionStateBase
	{
	public:

		ActionReload	()				{}
		~ActionReload	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
		void Exit	(Enemy_3& _owner)	override;
	};

	class ActionDeath : public ActionStateBase
	{
	public:

		ActionDeath		()				{}
		~ActionDeath	()	override	{}

		void Enter	(Enemy_3& _owner)	override;
		void Update	(Enemy_3& _owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> _nextState);
	std::shared_ptr<ActionStateBase> m_nowState = nullptr;
};
