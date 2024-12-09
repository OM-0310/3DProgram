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

	void SearchProc		();	// 索敵処理
	void ChaseProc		();	// 追跡処理
	void RouteSearchProc(); // 経路探索処理

	void InitializeGrid	();	// グリッドの初期化処理
	void MoveAlongPath	();	// 敵を経路に沿って次の位

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
	void SetHPDec				(const int& _decNum) 
	{ m_HP -= _decNum; }

	int GetHP			()	override { return m_HP; }

	enum class StateType
	{
		Normal,				// 通常
		Wary,				// 警戒
		Discover			// 発見
	};

	enum class SituationType
	{
		Idle	= 1 << 0,	// 停止状態
		Walk	= 1 << 1,	// 歩行状態
		Run		= 1 << 2,	// 走行状態
		Ready	= 1 << 3,	// 構え状態
		Shot	= 1 << 4,	// 発射状態
		Reload	= 1 << 5	// リロード状態
	};

private:

	std::shared_ptr<KdAnimator>			m_spAnimator;
	std::weak_ptr<Enemy_Main>			m_wpEnemy_Main;
	std::weak_ptr<Enemy_Gun>			m_wpEnemy_Gun;
	std::weak_ptr<Enemy_Gun_NoMagazine>	m_wpEnemy_Gun_NoMagazine;
	std::weak_ptr<Enemy_Magazine>		m_wpEnemy_Magazine;
	std::weak_ptr<Player>				m_wpPlayer;

	Math::Vector3				m_moveDir		= Math::Vector3::Zero;
	float						m_moveSpeed		= 0.0f;

	bool						m_waryFlg		= false;
	bool						m_discoverFlg	= false;
	bool						m_chaseFlg		= false;

	int							m_gridWidth		= 0;
	int							m_gridHeight	= 0;

	static const int			MAXHP = 1;

	StateType					m_sType			= StateType::Normal;
	SituationType				m_sitType		= SituationType::Idle;

	std::shared_ptr<AstarGrid>				m_spGrid; // グリッドのポインタをクラスメンバとして保持
	std::vector<std::shared_ptr<AstarNode>> m_spPath; // 現在のパスを保持する

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
		void Exit	(Enemy& _owner)	override;
	};

	class ActionWalk : public ActionStateBase
	{
	public:

		ActionWalk	()				{}
		~ActionWalk	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
		void Exit	(Enemy& _owner)	override;
	};

	class ActionRun : public ActionStateBase
	{
	public:

		ActionRun	()				{}
		~ActionRun	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
		void Exit	(Enemy& _owner)	override;
	};

	class ActionReady : public ActionStateBase
	{
	public:

		ActionReady	()				{}
		~ActionReady()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
		void Exit	(Enemy& _owner)	override;
	};

	class ActionShot : public ActionStateBase
	{
	public:

		ActionShot	()				{}
		~ActionShot	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
		void Exit	(Enemy& _owner)	override;
	};

	class ActionReload : public ActionStateBase
	{
	public:

		ActionReload	()				{}
		~ActionReload	()	override	{}

		void Enter	(Enemy& _owner)	override;
		void Update	(Enemy& _owner)	override;
		void Exit	(Enemy& _owner)	override;
	};

	void ChangeActionState(std::shared_ptr<ActionStateBase> _nextState);
	std::shared_ptr<ActionStateBase> m_nowState = nullptr;
};
