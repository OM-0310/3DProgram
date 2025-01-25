#include "Enemy_Gun_2.h"

#include "../Enemy_2.h"

#include "../../../../../main.h"

void Enemy_Gun_2::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Enemy/Enemy_Gun/Enemy_Gun.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,0.f,1.0f };

	m_nowBullet = m_magazineSize;

	m_alpha = m_alphaMax;
}

void Enemy_Gun_2::Update()
{
	const std::shared_ptr<Enemy_2> spEnemy = m_wpEnemy.lock();

	if (spEnemy)
	{
		m_mWorld = spEnemy->GetMatrix();

		if (spEnemy->GetDissolveFlg())
		{
			m_isExpired = true;
		}
	}

	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Enemy_Gun_2::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}

void Enemy_Gun_2::DrawLit()
{
	if (!m_spModel)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void Enemy_Gun_2::GenerateDepthMapFromLight()
{
	if (!m_spModel)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}
