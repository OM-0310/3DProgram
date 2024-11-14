#include "Player_UpperBody.h"

#include "../Player.h"

void Player_UpperBody::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_UpperBody.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,-0.9f,-50.0f };
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