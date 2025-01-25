#include "Enemy_Gun_NoMagazine_3.h"

#include "../Enemy_3.h"

void Enemy_Gun_NoMagazine_3::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Enemy/Enemy_Gun_NoMagazine/Enemy_Gun_NoMagazine.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,0.f,3.0f };

	m_alpha = m_alphaMin;
}

void Enemy_Gun_NoMagazine_3::Update()
{
	const std::shared_ptr<Enemy_3> spEnemy = m_wpEnemy.lock();

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

void Enemy_Gun_NoMagazine_3::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}

void Enemy_Gun_NoMagazine_3::DrawLit()
{
	if (!m_spModel)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void Enemy_Gun_NoMagazine_3::GenerateDepthMapFromLight()
{
	if (!m_spModel)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}
