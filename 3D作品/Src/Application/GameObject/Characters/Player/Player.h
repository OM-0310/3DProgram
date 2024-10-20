#pragma once
#include "../CharaBase.h"

class TPSCamera;
class Pistol_Disarm;
class Pistol_Ready;
class AssaultRifle;
class Door;
class CardKey;
class Goal;
class SecretFile;
class Reticle;
class PlayerStateBase;

class Player : public CharaBase
{
public:

	enum class PlayerType
	{
		Idle,
		Walk,
		Run,
		Ready,
		Stand,
		Sit,
		Sit_Idle,
		Sit_Walk,
		Sit_Ready,
		Creep,
		Creep_Idle,
		Creep_Walk,
		Creep_Ready,
		Roll
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
		CardKeyCollect		= 1 << 1,	// アイテム保持
		SecretFileCollect	= 1 << 2
	};

	Player						()				{}
	~Player						()	override	{ Release(); }

	void Init					()	override;
	void Update					()	override;
	void PostUpdate				()	override;

	void UpdateCollision		();

	void ChanegeAnimation		(const std::string& _name);	// アニメーション切り替え処理
	void ShotBullet				(const Math::Vector3& _pos, const Math::Vector3& _dir);
	void StateIdleProc			();
	void StateWalkProc			();
	void StateRunProc			();
	void StateReadyProc			();
	void StateShotProc			();
	void StateStandProc			();
	void StateSitProc			();
	void StateSitIdleProc		();
	void StateSitWalkProc		();
	void StateSitReadyProc		();
	void MoveProcess			();	// 移動処理
	void AvoidProcess			();	// 回避処理
	void ChanegeViewPointProcess();	// 視点切り替え処理
	void ChanegePostureProcess	();	// 体勢切り替え処理
	void AimProcess				();	// 銃構え処理
	void ShotProcess			();	// 弾発射処理
	void ChanegeWeaponProcess	();	// 武器切り替え処理

	void SetCamera		(const std::shared_ptr<TPSCamera>& camera)
	{
		m_wpCamera = camera;
	}

	void SetPistolDisarm(const std::shared_ptr<Pistol_Disarm>& _spPistol_Disarm)
	{
		m_wpPistol_Disarm = _spPistol_Disarm;
	}

	void SetPistolReady	(const std::shared_ptr<Pistol_Ready>& _spPistol_Ready)
	{
		m_wpPistol_Ready = _spPistol_Ready;
	}

	void SetAssault		(const std::shared_ptr<AssaultRifle>& _spAssault)
	{
		m_wpAssault = _spAssault;
	}

	void SetCardKey		(const std::shared_ptr<CardKey>& _spCard)
	{
		m_wpCard = _spCard;
	}

	void SetDoor		(const std::shared_ptr<Door>& _spDoor)
	{
		m_wpDoor = _spDoor;
	}

	void SetSecretFile	(const std::shared_ptr<SecretFile>& _spFile)
	{
		m_wpFile = _spFile;
	}

	void SetGoal		(const std::shared_ptr<Goal>& _spGoal)
	{
		m_wpGoal = _spGoal;
	}

	void SetReticle		(const std::shared_ptr<Reticle>& reticle)
	{
		m_wpReticle = reticle;
	}

	PlayerType GetPlayerType				() const	{ return m_pType; }

	const bool& GetHoldFlg					()			{ return m_holdFlg; }
	const Math::Matrix& GetRotateMat		()			{ return m_mRot; }
	const Math::Vector3& GetDisarmPos		()			{ return m_disarmPos; }
	const Math::Vector3& GetReadyPos		()			{ return m_readyPos; }
	const ItemCollectType& GetItemCollType	()			{ return m_itemCollType; }

private:

	std::weak_ptr<TPSCamera>	m_wpCamera;			// カメラ情報
	std::weak_ptr<Reticle>		m_wpReticle;		// レティクル情報
	std::weak_ptr<Pistol_Disarm>m_wpPistol_Disarm;	// ピストル(武装解除時)情報
	std::weak_ptr<Pistol_Ready>	m_wpPistol_Ready;	// ピストル(武装時)情報
	std::weak_ptr<AssaultRifle>	m_wpAssault;		// アサルト情報
	std::weak_ptr<Door>			m_wpDoor;			// ドア情報
	std::weak_ptr<CardKey>		m_wpCard;			// カードキー情報
	std::weak_ptr<SecretFile>	m_wpFile;			// 機密データ情報
	std::weak_ptr<Goal>			m_wpGoal;			// ゴール情報
	std::shared_ptr<KdAnimator> m_spAnimator;		// アニメーション

	Math::Matrix				m_mAdjustRotMat;	// 補正回転行列
	Math::Matrix				m_mHold;
	Math::Matrix				m_mUnHold;
	Math::Matrix				m_mLocalDisarm;
	Math::Matrix				m_mLocalReady;

	Math::Vector3				m_disarmPos;		// 武器座標(武装解除時)
	Math::Vector3				m_readyPos;			// 武器座標(武装時)
	Math::Vector3				m_moveDir;			// 移動方向
	float						m_moveSpeed;		// 移動速度

	bool						m_moveFlg;			// 移動フラグ
	bool						m_shotKeyFlg;		// エイムフラグ
	bool						m_holdFlg;			// 銃構えフラグ
	bool						m_keyFlg;			// キーフラグ
	bool						m_changeKeyFlg;		// 視点切り替え時用キーフラグ
	bool						m_posKeyFlg;		// しゃがみキーフラグ
	bool						m_creepKeyFlg;		// 匍匐キーフラグ

	static const int			STANDCNTMAX = 6;
	int							m_standCnt;
	static const int			SITCNTMAX = 6;
	int							m_sitCnt;
	static const int			SHOTCNTMAX = 12;
	int							m_shotCnt;

	static const int			MAXHP = 10;

	PlayerType					m_pType;
	PostureType					m_posType;
	ItemCollectType				m_itemCollType;
	POINT						m_FixMousePos;		// カメラ回転用マウス座標の原点

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
