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
		Run		= 1 << 1,	// 走行状態
		Walk	= 1 << 2,	// 歩行状態
		Ready	= 1 << 3,	// 構え状態
		Shot	= 1 << 4,	// 銃発射状態
		Reload	= 1 << 5,	// リロード状態
		Avoid	= 1 << 6	// 回避状態
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

	// アニメーション切り替え処理
	void ChangeUpperBodyAnimation(const std::string& _animeName, bool _isLoop = true, float _time = 0.0f);// 上半身
	void ChangeLowerBodyAnimation(const std::string& _animeName, bool _isLoop = true, float _time = 0.0f);// 下半身

	void ChanegeViewPointProcess();	// 視点切り替え処理

	void SetCamera					// カメラ情報セット		
	(const std::shared_ptr<TPSCamera>& _spCamera) { m_wpCamera = _spCamera; }

	void SetPlayerUpperBody			// プレイヤー上半身情報セット
	(std::shared_ptr<Player_UpperBody>& _spPlayer_Up) { m_wpPlayer_Up = _spPlayer_Up; }

	void SetPlayerLowerBody			// プレイヤー下半身情報セット
	(std::shared_ptr<Player_LowerBody>& _spPlayer_Low) { m_wpPlayer_Low = _spPlayer_Low; }

	void SetPlayerDisarm			// 拳銃ホルダー情報セット
	(std::shared_ptr<Player_Disarm>& _spPlayer_Disarm) { m_wpPlayer_Disarm = _spPlayer_Disarm; }
		
	void SetPlayerDisarmPistol		// ホルダーにしまわれている拳銃情報セット
	(std::shared_ptr<Player_Disarm_Pistol>& _spPlayer_Disarm_Pistol) { m_wpPlayer_Disarm_Pistol = _spPlayer_Disarm_Pistol; }

	void SetPlayerReadyPistol		// 構え時の銃情報セット
	(const std::shared_ptr<Player_Ready_Pistol>& _spPlayer_Ready_Pistol) { m_wpPlayer_Ready_Pistol = _spPlayer_Ready_Pistol; }

	void SetCardKey					// カードキー情報セット
	(const std::shared_ptr<CardKey>& _spCard) { m_wpCard = _spCard; }

	void SetDoor					// 鍵のかかったドア情報セット
	(const std::shared_ptr<LockedDoor>& _spLockDoor) { m_wpLockDoor = _spLockDoor; }

	void SetSecretFile				// 機密ファイル情報セット
	(const std::shared_ptr<SecretFile>& _spFile) { m_wpFile = _spFile; }

	void SetGoal					// ゴール情報セット
	(const std::shared_ptr<Goal>& _spGoal) { m_wpGoal = _spGoal; }

	void SetReticle					// レティクル情報セット
	(const std::shared_ptr<Reticle>& reticle) { m_wpReticle = reticle; }

	const bool&				GetHoldFlg			()			{ return m_holdFlg;			}
	const Math::Matrix&		GetRotateMat		()			{ return m_mRot;			}
	const ItemCollectType&	GetItemCollType		() const	{ return m_itemCollType;	}
	const UINT&				GetSituationType	() const	{ return m_sType;			}
	const PostureType&		GetPostureType		() const	{ return m_posType;			}

private:

	std::weak_ptr<TPSCamera>			m_wpCamera;					// カメラ情報
	std::weak_ptr<Reticle>				m_wpReticle;				// レティクル情報
	std::weak_ptr<Player_UpperBody>		m_wpPlayer_Up;				// プレイヤー上半身情報
	std::weak_ptr<Player_LowerBody>		m_wpPlayer_Low;				// プレイヤー下半身情報
	std::weak_ptr<Player_Disarm>		m_wpPlayer_Disarm;			// プレイヤー(武装解除)情報 
	std::weak_ptr<Player_Disarm_Pistol> m_wpPlayer_Disarm_Pistol;	// 銃(武装解除)情報 
	std::weak_ptr<Player_Ready_Pistol>	m_wpPlayer_Ready_Pistol;	// 銃(武装)情報 

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

	const float							m_walkMoveSpd	= 0.05f;	// 歩行速度 0.05f
	const float							m_runMoveSpd	= 0.12f;	// 走行速度 0.12f
	const float							m_sitWalkMoveSpd= 0.08f;	// しゃがみ歩行速度 0.08f

	bool								m_moveFlg		= false;	// 移動フラグ
	bool								m_shotKeyFlg	= false;	// エイムフラグ
	bool								m_reloadKeyFlg	= false;	// リロードフラグ
	bool								m_holdFlg		= false;	// 銃構えフラグ
	bool								m_keyFlg		= false;	// キーフラグ
	bool								m_changeKeyFlg	= false;	// 視点切り替え時用キーフラグ
	bool								m_posKeyFlg		= false;	// しゃがみキーフラグ
	bool								m_creepKeyFlg	= false;	// 匍匐キーフラグ
	bool								m_animFlg		= false;	// アニメ－ション切り替えフラグ

	static const int					MAXHP			= 100;		// 最大HP 100

	UINT								m_sType			= SituationType::Idle;			// プレイヤーの状態タイプ
	PostureType							m_posType		= PostureType::Stand;			// プレイヤーの体勢タイプ
	ItemCollectType						m_itemCollType	= ItemCollectType::NoCollect;	// アイテム所持タイプ

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
