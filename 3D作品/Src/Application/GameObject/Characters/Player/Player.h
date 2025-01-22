#pragma once
#include "../CharaBase.h"

class TPSCamera;
class Player_Main;
class Player_Disarm;
class Player_Disarm_Pistol;
class Player_Ready_Pistol;
class Enemy;
class LockedDoor;
class CardKey;
class Goal;
class SecretFile;
class Reticle;
class MiniMapUI;
class MiniMapUIBack;
class CurrentLocation;
class CardKeyLocation;
class SecretFileLocation;
class PlayerStateBase;

class Player : public CharaBase
{
public:

	enum SituationType
	{
		Idle			= 1 << 0,	// 停止状態
		Run				= 1 << 1,	// 走行状態
		Walk			= 1 << 2,	// 歩行状態
		Ready			= 1 << 3,	// 構え状態
		Shot			= 1 << 4,	// 銃発射状態
		Reload			= 1 << 5,	// リロード状態
		Restraint		= 1 << 6,	// 拘束状態
		Restraint_Idle	= 1 << 7,	// 拘束停止状態
	};

	enum class PostureType
	{
		Stand,		// 直立
		Sit,		// しゃがみ
		Creep		// 匍匐
	};

	enum class ItemCollectType
	{
		NoCollect,			// アイテム未保持
		CardKeyCollect,		// カードキー保持
		SecretFileCollect	// 機密ファイル保持
	};

	Player						()				{}
	~Player						()	override	{ Release(); }

	void Init					()	override;
	void Update					()	override;
	void PostUpdate				()	override;

	void UpdateCollision		(); // 各当たり判定処理
	void OpneMapProc			();	// マップ展開処理
	void CollectItemProc		();	// アイテム回収処理
	void AutoReloadProc			();	// 自動リロード処理
	void ChanegeViewPointProc	();	// 視点切り替え処理

	// アニメーション切り替え処理
	void ChangeAnimation(const std::string& _animeName, bool _isLoop = true, float _time = 0.0f);

	void SetCamera					// カメラ情報セット		
	(const std::shared_ptr<TPSCamera>& _spCamera) { m_wpCamera = _spCamera; }

	void SetPlayer_Main				// プレイヤー上半身情報セット
	(std::shared_ptr<Player_Main>& _spPlayer_Main) { m_wpPlayer_Main = _spPlayer_Main; }

	void SetPlayer_Disarm			// 拳銃ホルダー情報セット
	(std::shared_ptr<Player_Disarm>& _spPlayer_Disarm) { m_wpPlayer_Disarm = _spPlayer_Disarm; }
		
	void SetPlayer_Disarm_Pistol	// ホルダーにしまわれている拳銃情報セット
	(std::shared_ptr<Player_Disarm_Pistol>& _spPlayer_Disarm_Pistol) { m_wpPlayer_Disarm_Pistol = _spPlayer_Disarm_Pistol; }

	void SetPlayer_Ready_Pistol		// 構え時の銃情報セット
	(const std::shared_ptr<Player_Ready_Pistol>& _spPlayer_Ready_Pistol) { m_wpPlayer_Ready_Pistol = _spPlayer_Ready_Pistol; }

	void SetEnemy					// 敵情報セット
	(const std::shared_ptr<Enemy>& _spEnemy) { m_wpEnemy = _spEnemy; }

	void SetCardKey					// カードキー情報セット
	(const std::shared_ptr<CardKey>& _spCard) { m_wpCard = _spCard; }

	void SetDoor					// 鍵のかかったドア情報セット
	(const std::shared_ptr<LockedDoor>& _spLockDoor) { m_wpLockDoor = _spLockDoor; }

	void SetSecretFile				// 機密ファイル情報セット
	(const std::shared_ptr<SecretFile>& _spFile) { m_wpFile = _spFile; }

	void SetGoal					// ゴール情報セット
	(const std::shared_ptr<Goal>& _spGoal) { m_wpGoal = _spGoal; }

	void SetReticle					// レティクル情報セット
	(const std::shared_ptr<Reticle>& _spReticle) { m_wpReticle = _spReticle; }

	void SetMiniMapUI				// ミニマップ情報セット
	(const std::shared_ptr<MiniMapUI>& _spMiniMapUI) { m_wpMiniMapUI = _spMiniMapUI; }

	void SetMiniMapUIBack			// ミニマップ背景情報セット
	(const std::shared_ptr<MiniMapUIBack> _spMiniMapUIBack) { m_wpMiniMapUIBack = _spMiniMapUIBack; }

	void SetCurrentLocation			// 現在地UI情報セット
	(const std::shared_ptr<CurrentLocation>& _spCurrentLocation) { m_wpCurrentLocation = _spCurrentLocation; }

	void SetCardKeyLocation			// カードキー位置情報セット
	(const std::shared_ptr<CardKeyLocation>& _spCardKeyLocation) { m_wpCardKeyLocation = _spCardKeyLocation; }

	void SetSecretFileLocation		// 機密ファイル位置情報セット
	(const std::shared_ptr<SecretFileLocation>& _spSecretFileLocation) { m_wpSecretFileLocation = _spSecretFileLocation; }

	const Math::Matrix&		GetRotateMat		()			{ return m_mRot;			}
	const ItemCollectType&	GetItemCollType		() const	{ return m_itemCollType;	}
	const UINT&				GetSituationType	() const	{ return m_sType;			}
	const PostureType&		GetPostureType		() const	{ return m_posType;			}

private:

	std::weak_ptr<TPSCamera>			m_wpCamera;					// カメラ情報
	std::weak_ptr<Player_Main>			m_wpPlayer_Main;			// プレイヤー本体情報
	std::weak_ptr<Player_Disarm>		m_wpPlayer_Disarm;			// プレイヤー(武装解除)情報 
	std::weak_ptr<Player_Disarm_Pistol> m_wpPlayer_Disarm_Pistol;	// 銃(武装解除)情報 
	std::weak_ptr<Player_Ready_Pistol>	m_wpPlayer_Ready_Pistol;	// 銃(武装)情報 

	std::weak_ptr<Enemy>				m_wpEnemy;					// 敵情報
	std::weak_ptr<Reticle>				m_wpReticle;				// レティクル情報
	std::weak_ptr<MiniMapUI>			m_wpMiniMapUI;				// ミニマップ情報
	std::weak_ptr<MiniMapUIBack>		m_wpMiniMapUIBack;			// ミニマップ背景情報
	std::weak_ptr<CurrentLocation>		m_wpCurrentLocation;		// 現在地UI情報
	std::weak_ptr<CardKeyLocation>		m_wpCardKeyLocation;		// カードキー位置UI情報
	std::weak_ptr<SecretFileLocation>	m_wpSecretFileLocation;		// 機密ファイルUI情報
	std::weak_ptr<LockedDoor>			m_wpLockDoor;				// ドア情報
	std::weak_ptr<CardKey>				m_wpCard;					// カードキー情報
	std::weak_ptr<SecretFile>			m_wpFile;					// 機密データ情報
	std::weak_ptr<Goal>					m_wpGoal;					// ゴール情報

	std::shared_ptr<KdAnimator>			m_spAnimator;				// アニメーション
	float								m_nowAnimeFrm	= 0.0f;		// 現在のアニメーションフレーム
	float								m_animFrmSpd	= 1.0f;		// アニメーションの1フレーム(blender側では24fpsでのアニメーション実装のため)

	const float							m_reloadFrmMax	= 60.0f;	// リロードフレーム最大値 60.0f
	const float							m_standFrmMax	= 15.0f;	// 立ち上がりフレーム最大値 15.0f
	const float							m_sitFrmMax		= 15.0f;	// しゃがみフレーム最大値 15.0f
	const float							m_shotFrmMax	= 25.0f;	// 銃発砲フレーム最大値 25.0f
	const float							m_bulletShotFrm = 2.5f;		// 弾発射フレーム 2.5f
	const float							m_restFrmMax	= 32.5f;	// 拘束フレーム最大値 32.5f
	const float							m_grabFrm		= 7.5f;		// 掴み時フレーム 17.5f

	const float							m_zeroMoveSpd	= 0.0f;		// 停止速度 0.0f
	const float							m_walkMoveSpd	= 0.05f;	// 歩行速度 0.05f
	const float							m_runMoveSpd	= 0.12f;	// 走行速度 0.12f
	const float							m_sitWalkMoveSpd= 0.08f;	// しゃがみ歩行速度 0.08f

	const float							m_tightArea		= 3.0f;		// 拘束エリア 0.3f

	bool								m_openMapKeyFlg = false;	// マップ展開キーフラグ
	bool								m_shotKeyFlg	= false;	// エイムフラグ
	bool								m_reloadKeyFlg	= false;	// リロードフラグ
	bool								m_keyFlg		= false;	// キーフラグ
	bool								m_changeKeyFlg	= false;	// 視点切り替え時用キーフラグ
	bool								m_posKeyFlg		= false;	// しゃがみキーフラグ

	UINT								m_sType			= SituationType::Idle;			// プレイヤーの状態タイプ
	PostureType							m_posType		= PostureType::Stand;			// プレイヤーの体勢タイプ
	ItemCollectType						m_itemCollType	= ItemCollectType::NoCollect;	// アイテム所持タイプ

private:

	// ステートパターン管理系
	class ActionStateBase
	{
	public:

		virtual ~ActionStateBase()			{}

		virtual void Enter	(Player& _owner) { (void)_owner; }
		virtual void Update	(Player& _owner) { (void)_owner; }
		virtual void Exit	(Player& _owner) { (void)_owner; }
	};

	class ActionIdle : public ActionStateBase
	{
	public:

		ActionIdle	()				{}
		~ActionIdle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionWalk : public ActionStateBase
	{
	public:

		ActionWalk	()				{}
		~ActionWalk	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionRun : public ActionStateBase
	{
	public:

		ActionRun	()				{}
		~ActionRun	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReady : public ActionStateBase
	{
	public:

		ActionReady	()				{}
		~ActionReady()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionShot : public ActionStateBase
	{
	public:

		ActionShot	()				{}
		~ActionShot	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionRestraint : public ActionStateBase
	{
	public:

		ActionRestraint	()				{}
		~ActionRestraint()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionRestraint_Idle : public ActionStateBase
	{
	public:

		ActionRestraint_Idle	()				{}
		~ActionRestraint_Idle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReload_Idle : public ActionStateBase
	{
	public:

		ActionReload_Idle	()				{}
		~ActionReload_Idle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReload_Walk : public ActionStateBase
	{
	public:

		ActionReload_Walk	()				{}
		~ActionReload_Walk	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionReload_Run : public ActionStateBase
	{
	public:

		ActionReload_Run	()				{}
		~ActionReload_Run()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionStand : public ActionStateBase
	{
	public:

		ActionStand	()				{}
		~ActionStand()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit : public ActionStateBase
	{
	public:

		ActionSit	()				{}
		~ActionSit	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit_Idle : public ActionStateBase
	{
	public:

		ActionSit_Idle	()				{}
		~ActionSit_Idle	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit_Walk : public ActionStateBase
	{
	public:

		ActionSit_Walk	()				{}
		~ActionSit_Walk	()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};

	class ActionSit_Ready : public ActionStateBase
	{
	public:

		ActionSit_Ready	()				{}
		~ActionSit_Ready()	override	{}

		void Enter	(Player& _owner)	override;
		void Update	(Player& _owner)	override;
		void Exit	(Player& _owner)	override;
	};


	void ChangeActionState(std::shared_ptr<ActionStateBase> _nextState);
	std::shared_ptr<ActionStateBase> m_nowState = nullptr;
};
