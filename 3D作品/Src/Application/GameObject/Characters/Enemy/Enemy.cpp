#include "Enemy.h"

#include "../Player/Player.h"

#include "../../../System/Navigation/AstarNode/AstarNode.h"
#include "../../../System/Navigation/AstarGrid/AstarGrid.h"
#include "../../../System/Navigation/PathFinding/PathFinding.h"

void Enemy::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Enemy/Soldier.gltf");

	m_waryFlg		= false;
	m_discoverFlg	= false;
	m_chaseFlg		= false;

	m_HP			= MAXHP;

	m_pos			= { 0.f,0.f,1.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.05f;

	m_gridWidth		= 10;
	m_gridHeight	= 10;

	m_objectType	= ObjectType::TypeEnemy;

	//InitializeGrid();

	m_spAnimator	= std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Idle"));

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("EnemyCollision", m_spModel, KdCollider::TypeDamage);

	CharaBase::Init();
}

void Enemy::Update()
{
	SearchProc();

	ChaseProc();

	//RouteSearchProc();

	if (m_HP == 0)
	{
		m_isExpired = true;
	}

	m_pos.y -= m_gravity;
	m_gravity += 0.04f;

	m_moveDir.Normalize();

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);

	m_pos += m_moveDir * m_moveSpeed;

	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;
}

void Enemy::PostUpdate()
{
	// アニメーションの更新
	// 第二引数でアニメーションの速度を設定可能(デフォルトで1.0fに設定されている)
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();

	CharaBase::PostUpdate();
}

void Enemy::ChangeAnimation()
{

}

//=====================================================================================================
// 索敵処理・・・ここから
//=====================================================================================================
void Enemy::SearchProc()
{
	std::shared_ptr<Player> _spPlayer = m_wpPlayer.lock();
	Math::Vector3 _targetPos;
	if (_spPlayer)
	{
		_targetPos = _spPlayer->GetPos();
		Math::Vector3 _targetVec = _targetPos - m_pos;
		Math::Vector3 _nowVec = Math::Vector3::TransformNormal({ 0,0,1 }, m_mRot);
		float _innerView = _nowVec.Dot(_targetVec);

		if (_innerView > 0.25f)
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

