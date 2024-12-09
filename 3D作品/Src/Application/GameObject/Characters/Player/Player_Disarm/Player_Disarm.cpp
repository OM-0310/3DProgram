#include "Player_Disarm.h"

#include "../Player.h"

void Player_Disarm::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_Disarm_Holder.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos		= { 0.f,-0.9f,-50.0f };

	m_objectType = KdGameObject::ObjectType::TypePlayer;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player_DisarmCollsion", m_spModel, KdCollider::TypeEvent | KdCollider::TypeBump);
}

void Player_Disarm::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		m_mWorld = spPlayer->GetMatrix();
	}
}

void Player_Disarm::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}

void Player_Disarm::DrawUnLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
