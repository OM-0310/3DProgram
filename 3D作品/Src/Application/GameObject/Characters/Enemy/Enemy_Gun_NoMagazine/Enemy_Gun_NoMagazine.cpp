#include "Enemy_Gun_NoMagazine.h"

#include "../Enemy.h"

void Enemy_Gun_NoMagazine::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Enemy/Enemy_Gun_NoMagazine/Enemy_Gun_NoMagazine.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,0.f,1.0f };

	m_alpha = m_alphaMin;
}

void Enemy_Gun_NoMagazine::Update()
{
	std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();

	if (spEnemy)
	{
		m_mWorld = spEnemy->GetMatrix();
	}
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Enemy_Gun_NoMagazine::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}
