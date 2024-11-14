#include "Player_LowerBody.h"

#include "../Player.h"

void Player_LowerBody::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_LowerBody.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,-0.9f,-50.0f };
}

void Player_LowerBody::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		m_mWorld = spPlayer->GetMatrix();
	}
}

void Player_LowerBody::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}
