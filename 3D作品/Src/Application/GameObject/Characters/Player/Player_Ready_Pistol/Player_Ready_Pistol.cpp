#include "Player_Ready_Pistol.h"

#include "../Player.h"

void Player_Ready_Pistol::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_Ready_Pistol.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,-0.9f,-50.0f };

	m_alpha = m_alphaMin;
}

void Player_Ready_Pistol::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		m_mWorld = spPlayer->GetMatrix();

		if (spPlayer->GetSituationType() == Player::SituationType::Ready ||
			spPlayer->GetSituationType() == Player::SituationType::Reload)
		{
			m_alpha = m_alphaMax;
		}
		else
		{
			m_alpha = m_alphaMin;
		}
	}
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Player_Ready_Pistol::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}

void Player_Ready_Pistol::DrawUnLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}
