#pragma once
#include "../CharaBase.h"

class TPSCamera;
class Player_UpperBody;
class Player_LowerBody;
class Player_Disarm;
class Player_Disarm_Pistol;
class Player_Ready_Pistol;
class Pistol_Disarm;
class Pistol_Ready;
class AssaultRifle;
class LockedDoor;
class CardKey;
class Goal;
class SecretFile;
class Reticle;
class PlayerStateBase;

class Player : public CharaBase
{
public:

	enum SituationType
	{
		Idle	= 1 << 0,	// 停止状態
		Move	= 1 << 1,	// 移動状態
		Ready	= 1 << 2,	// 構え状態
		Reload	= 1 << 3,	// リロード状態
		Avoid	= 1 << 4	// 回避状態
	};

	enum class PostureType
	{
		Stand,		// 直立
		Sit,		// しゃがみ
		Creep		// 匍匐
	};

	enum class ItemCollectType
	{
		NoCollect			= 1 << 0,	// アイテム未保持
		CardKeyCollect		= 1 << 1,	// カードキー保持
		SecretFileCollect	= 1 << 2	// 機密ファイル保持
	};

	Player						()				{}
	~Player						()	override	{ Release(); }

	void Init					()	override;
	void Update					()	override;
	void PostUpdate				()	override;

	void UpdateCollision		();

	void ChangeAnimation(const std::string& _animeName);
	void ChangeAnimation(const std::string& _upperBody_Name, const std::string& _lowerBody_Name);

	void ShotBullet		(const Math::Vector3& _pos, const Math::Vector3& _dir);

	void StateIdleProc			();
	void StateWalkProc			();
	void StateRunProc			();
	void StateReadyProc			();
	void StateShotProc			();
	void StateReloadProc		();
	void StateStandProc			();
	void StateSitProc			();
	void StateSitIdleProc		();
	void StateSitWalkProc		();
	void StateSitReadyProc		();
	void ChanegeViewPointProcess();	// 視点切り替え処理

	void SetCamera				(const std::shared_ptr<TPSCamera>& _spCamera) 
	{ m_wpCamera = _spCamera; }

	void SetPlayerUpperBody		(std::shared_ptr<Player_UpperBody>& _spPlayer_Up) 
	{ m_wpPlayer_Up = _spPlayer_Up; }

	void SetPlayerLowerBody(std::shared_ptr<Player_LowerBody>& _spPlayer_Low) 
	{ m_wpPlayer_Low = _spPlayer_Low; }

	void SetPlayerDisarm		(std::shared_ptr<Player_Disarm>& _spPlayer_Disarm) 
	{ m_wpPlayer_Disarm = _spPlayer_Disarm; }

	void SetPlayerDisarmPistol	(std::shared_ptr<Player_Disarm_Pistol>& _spPlayer_Disarm_Pistol) 
	{ m_wpPlayer_Disarm_Pistol = _spPlayer_Disarm_Pistol; }

	void SetPistolDisarm		(std::shared_ptr<Pistol_Disarm>& _spPistol_Disarm) 
	{ m_wpPistol_Disarm = _spPistol_Disarm; }

	void SetPlayerReadyPistol	(const std::shared_ptr<Player_Ready_Pistol>& _spPlayer_Ready_Pistol) 
	{ m_wpPlayer_Ready_Pistol = _spPlayer_Ready_Pistol; }

	void SetPistolReady			(const std::shared_ptr<Pistol_Ready>& _spPistol_Ready) 
	{ m_wpPistol_Ready = _spPistol_Ready; }

	void SetAssault				(const std::shared_ptr<AssaultRifle>& _spAssault) 
	{ m_wpAssault = _spAssault; }

	void SetCardKey				(const std::shared_ptr<CardKey>& _spCard) 
	{ m_wpCard = _spCard; }

	void SetDoor				(const std::shared_ptr<LockedDoor>& _spLockDoor) 
	{ m_wpLockDoor = _spLockDoor; }

	void SetSecretFile			(const std::shared_ptr<SecretFile>& _spFile) 
	{ m_wpFile = _spFile; }

	void SetGoal				(const std::shared_ptr<Goal>& _spGoal) 
	{ m_wpGoal = _spGoal; }

	void SetReticle				(const std::shared_ptr<Reticle>& reticle) 
	{ m_wpReticle = reticle; }

	const bool&				GetHoldFlg			()			{ return m_holdFlg; }
	const Math::Matrix&		GetRotateMat		()			{ return m_mRot; }
	const Math::Vector3&	GetDisarmPos		()			{ return m_disarmPos; }
	const Math::Vector3&	GetReadyPos			()			{ return m_readyPos; }
	const ItemCollectType&	GetItemCollType		() const	{ return m_itemCollType; }
	const UINT&	GetSituationType				() const	{ return m_sType; }
	const PostureType&		GetPostureType		() const	{ return m_posType; }

private:

	std::weak_ptr<TPSCamera>			m_wpCamera;					// カメラ情報
	std::weak_ptr<Reticle>				m_wpReticle;				// レティクル情報
	std::weak_ptr<Player_UpperBody>		m_wpPlayer_Up;				// プレイヤー上半身情報
	std::weak_ptr<Player_LowerBody>		m_wpPlayer_Low;				// プレイヤー下半身情報
	std::weak_ptr<Player_Disarm>		m_wpPlayer_Disarm;			// プレイヤー(武装解除)情報 
	std::weak_ptr<Player_Disarm_Pistol> m_wpPlayer_Disarm_Pistol;	// 銃(武装解除)情報 
	std::weak_ptr<Player_Ready_Pistol>	m_wpPlayer_Ready_Pistol;	// 銃(武装)情報 

	// 削除予定 // //// //// //// //// //// //// //// //// //// //// //// ////
	std::weak_ptr<Pistol_Disarm>m_wpPistol_Disarm;	// ピストル(武装解除時)情報
	std::weak_ptr<Pistol_Ready>	m_wpPistol_Ready;	// ピストル(武装時)情報
	std::weak_ptr<AssaultRifle>	m_wpAssault;		// アサルト情報
	//// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

	std::weak_ptr<LockedDoor>			m_wpLockDoor;				// ドア情報
	std::weak_ptr<CardKey>				m_wpCard;					// カードキー情報
	std::weak_ptr<SecretFile>			m_wpFile;					// 機密データ情報
	std::weak_ptr<Goal>					m_wpGoal;					// ゴール情報
	std::shared_ptr<KdAnimator>			m_spAnimator;				// アニメーション

	Math::Matrix						m_mAdjustRotMat;			// 補正回転行列

	// 削除予定 // //// //// //// //// //// //// //// //// //// //// //// ////
	Math::Matrix				m_mHold;
	Math::Matrix				m_mUnHold;
	Math::Matrix				m_mLocalDisarm;
	Math::Matrix				m_mLocalReady;

	Math::Vector3				m_disarmPos;		// 武器座標(武装解除時)
	Math::Vector3				m_readyPos;			// 武器座標(武装時)
	//// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

	bool								m_moveFlg;					// 移動フラグ
	bool								m_shotKeyFlg;				// エイムフラグ
	bool								m_reloadKeyFlg;				// リロードフラグ
	bool								m_holdFlg;					// 銃構えフラグ
	bool								m_keyFlg;					// キーフラグ
	bool								m_changeKeyFlg;				// 視点切り替え時用キーフラグ
	bool								m_posKeyFlg;				// しゃがみキーフラグ
	bool								m_creepKeyFlg;				// 匍匐キーフラグ
	bool								m_animFlg;					// アニメ－ション切り替えフラグ

	static const int					STANDCNTMAX		= 15;		// 立ち上がり時フレーム最大値
	int									m_standCnt;					// 立ち上がり時フレーム

	static const int					SITCNTMAX		= 15;		// しゃがみ時フレーム最大値
	int									m_sitCnt;					// しゃがみ時フレーム

	static const int					SHOTCNTMAX		= 48;		// 銃発射時フレーム最大値
	int									m_shotCnt;					// 銃発射時フレーム

	static const int					RELOADCNTMAX	= 60;		// リロード時フレーム最大値
	int									m_reloadCnt;				// リロード時フレーム

	static const int					MAXHP			= 100;		// 最大HP

	UINT								m_sType;					// プレイヤーの状態タイプ
	PostureType							m_posType;					// プレイヤーの体勢タイプ
	ItemCollectType						m_itemCollType;				// アイテム所持タイプ
	POINT								m_FixMousePos;				// カメラ回転用マウス座標の原点

private:

	// ステートパターン管理系
	class ActionStateBase
	{
	public:

		virtual ~ActionStateBase()			{}

		virtual void Enter	(Player& _owner){}
		virtual void Update	(Player& _owner){}
		virtual void Exit	(Player& _owner){}
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

	class ActionReload : public ActionStateBase
	{
	public:

		ActionReload	()				{}
		~ActionReload	()	override	{}

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
