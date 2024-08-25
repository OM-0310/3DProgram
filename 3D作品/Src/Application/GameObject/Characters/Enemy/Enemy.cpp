#include "Enemy.h"

#include "../Player/Player.h"

void Enemy::Init()
{
	m_spModel		= std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Characters/Enemy/Soldier.gltf");

	m_pos			= { 0.f,0.f,1.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.f;

	m_spAnimator	= std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Idle"));

	CharaBase::Init();
}

void Enemy::Update()
{
	SearchProcess();

	m_moveDir.Normalize();

	m_pos		+= m_moveDir * m_moveSpeed;

	m_pos.y		-= m_gravity;
	m_gravity	+= 0.04f;

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);

	m_mRot		= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld	= m_mRot * m_mTrans;
}

void Enemy::PostUpdate()
{
	// アニメーションの更新
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();

	CharaBase::PostUpdate();
}

//================================================================================================================================
// 索敵処理・・・ここから
//================================================================================================================================
void Enemy::SearchProcess()
{
	std::shared_ptr<Player> _spPlayer = m_wpPlayer.lock();
	Math::Vector3			_targetVec;
	Math::Vector3			_targetPos;
	if (_spPlayer)
	{
		float _innerView = _targetVec.Dot(m_moveDir);

		if (_innerView > 0.4f)
		{

		}
	}
}
//================================================================================================================================
// 索敵処理・・・ここまで
//================================================================================================================================
