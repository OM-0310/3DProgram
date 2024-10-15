#pragma once
#include "../CharaBase.h"

class TPSCamera;
class WeaponBase;
class Pistol;
class AssaultRifle;
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

	void SetCamera				(const std::shared_ptr<TPSCamera>& camera)
	{
		m_wpCamera = camera;
	}

	void SetWeapon				(const std::shared_ptr<WeaponBase>& weapon)
	{
		m_wpWeapon = weapon;
	}

	void SetReticle				(const std::shared_ptr<Reticle>& reticle)
	{
		m_wpReticle = reticle;
	}

	PlayerType GetPlayerType() const { return m_pType; }

private:

	std::weak_ptr<TPSCamera>	m_wpCamera;		// カメラ情報
	std::weak_ptr<Reticle>		m_wpReticle;	// レティクル情報
	std::weak_ptr<WeaponBase>	m_wpWeapon;		// 武器情報
	std::shared_ptr<KdAnimator> m_spAnimator;	// アニメーション

	Math::Matrix				m_mAdjustRotMat;// 補正回転行列
	Math::Matrix				m_mHold;
	Math::Matrix				m_mUnHold;

	Math::Vector3				m_moveDir;		// 移動方向
	float						m_moveSpeed;	// 移動速度

	bool						m_moveFlg;		// 移動フラグ
	bool						m_shotFlg;		// エイムフラグ
	bool						m_keyFlg;		// キーフラグ
	bool						m_changeKeyFlg;	// 視点切り替え時用キーフラグ
	bool						m_posKeyFlg;	// しゃがみキーフラグ
	bool						m_creepKeyFlg;	// 匍匐キーフラグ

	static const int			STANDCNTMAX = 6;
	int							m_standCnt;
	static const int			SITCNTMAX = 6;
	int							m_sitCnt;
	static const int			SHOTCNTMAX = 12;
	int							m_shotCnt;

	PlayerType					m_pType;
	PostureType					m_posType;
	POINT						m_FixMousePos;	// カメラ回転用マウス座標の原点

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
