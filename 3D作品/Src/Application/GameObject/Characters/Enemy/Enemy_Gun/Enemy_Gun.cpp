#include "Enemy_Gun.h"

#include "../Enemy.h"

void Enemy_Gun::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Enemy/Enemy_Gun/Enemy_Gun.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,0.f,1.0f };

	m_alpha = m_alphaMax;
}

void Enemy_Gun::Update()
{
	std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();

	if (spEnemy)
	{
		m_mWorld = spEnemy->GetMatrix();
	}
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Enemy_Gun::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}
