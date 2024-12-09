#include "Player_UpperBody.h"

#include "../Player.h"

void Player_UpperBody::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_UpperBody_3.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,-0.9f,-50.0f };

	m_objectType = KdGameObject::ObjectType::TypePlayer;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player_UpperBodyCollsion", m_spModel, KdCollider::TypeEvent | KdCollider::TypeBump);
}

void Player_UpperBody::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		m_mWorld = spPlayer->GetMatrix();
	}
}

void Player_UpperBody::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}