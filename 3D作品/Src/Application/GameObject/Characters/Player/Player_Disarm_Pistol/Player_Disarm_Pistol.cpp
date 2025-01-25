#include "Player_Disarm_Pistol.h"

#include "../Player.h"

void Player_Disarm_Pistol::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_Disarm_Pistol/Player_Disarm_Pistol.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,-0.9f,-50.0f };

	m_alpha = m_alphaMax;

	m_objectType = KdGameObject::ObjectType::TypePlayer;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player_Disarm_PistolCollsion", m_spModel, KdCollider::TypeEvent | KdCollider::TypeBump);
}

void Player_Disarm_Pistol::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		m_mWorld = spPlayer->GetMatrix();

		if (spPlayer->GetSituationType() & Player::SituationType::Ready				||
			spPlayer->GetSituationType() & Player::SituationType::Reload			||
			spPlayer->GetSituationType() & Player::SituationType::Restraint			||
			spPlayer->GetSituationType() & Player::SituationType::Restraint_Idle	||
			spPlayer->GetSituationType() & Player::SituationType::Kill				||
			spPlayer->GetSituationType() & Player::SituationType::Shot)
		{
			m_alpha = m_alphaMin;
		}
		else
		{
			m_alpha = m_alphaMax;
		}
	}
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Player_Disarm_Pistol::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}

void Player_Disarm_Pistol::DrawUnLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}
