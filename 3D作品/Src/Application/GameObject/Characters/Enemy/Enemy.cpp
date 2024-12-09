#include "Enemy.h"

#include "Enemy_Main/Enemy_Main.h"
#include "Enemy_Gun/Enemy_Gun.h"
#include "Enemy_Gun_NoMagazine/Enemy_Gun_NoMagazine.h"
#include "Enemy_Magazine/Enemy_Magazine.h"

#include "../Player/Player.h"

#include "../../../System/Navigation/AstarNode/AstarNode.h"
#include "../../../System/Navigation/AstarGrid/AstarGrid.h"
#include "../../../System/Navigation/PathFinding/PathFinding.h"

#include "../../../Scene/SceneManager.h"

void Enemy::Init()
{
	m_spAnimator	= std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_waryFlg		= false;
	m_discoverFlg	= false;
	m_chaseFlg		= false;

	m_HP			= MAXHP;

	m_pos			= { 0.f,-0.9f,1.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.05f;

	m_gridWidth		= 10;
	m_gridHeight	= 10;

	m_objectType	= ObjectType::TypeEnemy;

	//InitializeGrid();

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollision", m_spModel, KdCollider::TypeDamage);

	CharaBase::Init();
}

void Enemy::Update()
{
	//SearchProc();

	//ChaseProc();

	//RouteSearchProc();

	if (m_HP == 0)
	{
		m_isExpired = true;
	}

	m_pos.y -= m_gravity;
	m_gravity += 0.04f;	

	// 各種「状態」に応じた更新処理を実行する
	if (m_nowState)
	{
		m_nowState->Update(*this);
	}

	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;
}

void Enemy::PostUpdate()
{
	UpdateCollision();
}

void Enemy::UpdateCollision()
{
	//==========================================================
	// レイ判定
	//==========================================================

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	rayInfo.m_dir = Math::Vector3::Down;
	rayInfo.m_pos.y += 0.01f;

	float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;

	rayInfo.m_range = m_gravity + enableStepHigh;
	rayInfo.m_type = KdCollider::TypeGround;

	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}

	float maxOverLap = 0;
	Math::Vector3 hitPos;
	bool isHit = false;
	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			isHit = true;
		}
	}
	if (isHit)
	{
		m_pos = hitPos + Math::Vector3{ 0.f,-0.01f,0.f };
		m_gravity = 0;
	}

	//==========================================================
	// 球判定
	//==========================================================

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos + Math::Vector3{ 0.f,0.9f,0.f };
	sphere.m_sphere.Radius = 0.3f;
	sphere.m_type = KdCollider::TypeGround;
	// デバッグ用
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

	// 弾が当たったオブジェクトの情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	// 弾と当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphere, &retSphereList))
		{

		}
	}

	// 球リストから一番近いオブジェクトを検出
	maxOverLap = 0.f;
	isHit = false;
	Math::Vector3 hitDir;
	for (auto& ret : retSphereList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			isHit = true;
		}
	}
	if (isHit)
	{
		hitDir.Normalize();

		m_pos += hitDir * maxOverLap;
	}
}

void Enemy::ChangeAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	std::shared_ptr<Enemy_Main>				spEnemy_Main			= m_wpEnemy_Main.lock();
	std::shared_ptr<Enemy_Gun>				spEnemy_Gun				= m_wpEnemy_Gun.lock();
	std::shared_ptr<Enemy_Gun_NoMagazine>	spEnemy_Gun_NoMagazine	= m_wpEnemy_Gun_NoMagazine.lock();
	std::shared_ptr<Enemy_Magazine>			spEnemy_Magazine		= m_wpEnemy_Magazine.lock();
	if (spEnemy_Main)
	{
		spEnemy_Main->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spEnemy_Gun)
	{
		spEnemy_Gun->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spEnemy_Gun_NoMagazine)
	{
		spEnemy_Gun_NoMagazine->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spEnemy_Magazine)
	{
		spEnemy_Magazine->ChangeAnimation(_animeName, _isLoop, _time);
	}
}

//=====================================================================================================
// 索敵処理・・・ここから
//=====================================================================================================
void Enemy::SearchProc()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	Math::Vector3 targetPos;
	if (spPlayer)
	{
		targetPos = spPlayer->GetPos();
		Math::Vector3 targetVec = targetPos - m_pos;
		Math::Vector3 nowVec = Math::Vector3::TransformNormal({ 0,0,1 }, m_mRot);
		float innerView = nowVec.Dot(targetVec);

		if (innerView > 0.25f)
		{
			m_chaseFlg = true;
		}
	}
}
//=====================================================================================================
// 索敵処理・・・ここまで
//=====================================================================================================

//=====================================================================================================
// 追従処理・・・ここから
//=====================================================================================================
void Enemy::ChaseProc()
{
	std::shared_ptr<Player> _spPlayer = m_wpPlayer.lock();
	Math::Vector3 _targetPos;
	if (_spPlayer)
	{
		_targetPos = _spPlayer->GetPos();
	}
	if (m_chaseFlg)
	{
		m_moveDir = _targetPos - m_pos;
	}
}
//=====================================================================================================
// 追従処理・・・ここまで
//=====================================================================================================

//=====================================================================================================
// 経路探索処理・・・ここから
//=====================================================================================================
void Enemy::RouteSearchProc()
{
	// プレイヤーを発見していなければ早期リターン
	if (!m_chaseFlg)return;
	
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	Math::Vector3			playerPos;

	// プレイヤーの座標を取得
	if (spPlayer)
	{
		playerPos = spPlayer->GetPos();
	}

	// 敵とプレイヤーのノードを取得
	std::shared_ptr<AstarNode> enemyNode = m_spGrid->GetNode(static_cast<int>(m_pos.x), static_cast<int>(m_pos.z));
	std::shared_ptr<AstarNode> playerNode = m_spGrid->GetNode(static_cast<int>(playerPos.x), static_cast<int>(playerPos.z));

	// ノードが有効出なければ早期リターン
	if (!enemyNode || !playerNode)return;

	// A*アルゴリズムで経路を計算
	m_spPath = PathFinding::Astar(enemyNode, playerNode, *m_spGrid);

	// パスに沿って移動
	MoveAlongPath();
}
//=====================================================================================================
// 経路探索処理・・・ここまで
//=====================================================================================================

//=====================================================================================================
// 敵を経路に沿って次の位置に移動させる処理・・・ここから
//=====================================================================================================
void Enemy::MoveAlongPath()
{
	// 経路がなければ早期リターンで何もしない
	if (m_spPath.empty())return;

	// 次のステップに移動
	std::shared_ptr<AstarNode> nextStep = m_spPath.front();
	m_spPath.erase(m_spPath.begin());

	// 新しい位置に移動
	Math::Vector3 nextPos(static_cast<float>(nextStep->GetPosX()), 0.f, static_cast<float>(nextStep->GetPosZ()));

	m_moveDir = nextPos - m_pos;
	m_moveDir.Normalize();
	m_pos += m_moveDir * m_moveSpeed;

}
//=====================================================================================================
// 敵を経路に沿って次の位置に移動させる処理・・・ここまで
//=====================================================================================================

void Enemy::InitializeGrid()
{
	// グリッド作成
	m_spGrid = std::make_shared<AstarGrid>(m_gridWidth, m_gridHeight);

	// 障害物の設定
	m_spGrid->SetObstacle(3, 3, true);
	m_spGrid->SetObstacle(3, 4, true);
	m_spGrid->SetObstacle(3, 5, true);
}

void Enemy::ChangeActionState(std::shared_ptr<ActionStateBase> _nextState)
{
	if (m_nowState)m_nowState->Exit(*this);
	m_nowState = _nextState;
	m_nowState->Enter(*this);
}

void Enemy::ActionIdle::Enter(Enemy& _owner)
{
	if (_owner.m_sitType != SituationType::Idle)
	{
		_owner.m_sitType = SituationType::Idle;
		_owner.ChangeAnimation("Idle");
	}
}

void Enemy::ActionIdle::Update(Enemy& _owner)
{

}

void Enemy::ActionIdle::Exit(Enemy& _owner)
{
}

void Enemy::ActionWalk::Enter(Enemy& _owner)
{
	if (_owner.m_sitType != SituationType::Walk)
	{
		_owner.m_sitType = SituationType::Walk;
		_owner.ChangeAnimation("Walk");
	}
}

void Enemy::ActionWalk::Update(Enemy& _owner)
{
}

void Enemy::ActionWalk::Exit(Enemy& _owner)
{
}

void Enemy::ActionRun::Enter(Enemy& _owner)
{
	if (_owner.m_sitType != SituationType::Run)
	{
		_owner.m_sitType = SituationType::Run;
		_owner.ChangeAnimation("Run");
	}
}

void Enemy::ActionRun::Update(Enemy& _owner)
{
}

void Enemy::ActionRun::Exit(Enemy& _owner)
{
}

void Enemy::ActionReady::Enter(Enemy& _owner)
{
	if (_owner.m_sitType != SituationType::Ready)
	{
		_owner.m_sitType = SituationType::Ready;
		_owner.ChangeAnimation("Ready");
	}
}

void Enemy::ActionReady::Update(Enemy& _owner)
{
}

void Enemy::ActionReady::Exit(Enemy& _owner)
{
}

void Enemy::ActionShot::Enter(Enemy& _owner)
{
	if (_owner.m_sitType != SituationType::Shot)
	{
		_owner.m_sitType = SituationType::Shot;
		_owner.ChangeAnimation("Shot");
	}
}

void Enemy::ActionShot::Update(Enemy& _owner)
{
}

void Enemy::ActionShot::Exit(Enemy& _owner)
{
}

void Enemy::ActionReload::Enter(Enemy& _owner)
{
	if (_owner.m_sitType != SituationType::Reload)
	{
		_owner.m_sitType = SituationType::Reload;
		_owner.ChangeAnimation("Reload");
	}
}

void Enemy::ActionReload::Update(Enemy& _owner)
{
}

void Enemy::ActionReload::Exit(Enemy& _owner)
{
}
