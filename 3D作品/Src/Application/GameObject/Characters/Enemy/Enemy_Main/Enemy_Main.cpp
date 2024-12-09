#include "Enemy_Main.h"

#include "../Enemy.h"

void Enemy_Main::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Enemy/Enemy_Main/Enemy_Main.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,0.f,1.0f };
}

void Enemy_Main::Update()
{
	std::shared_ptr<Enemy> spEnemy = m_wpEnemy.lock();

	if (spEnemy)
	{
		m_mWorld = spEnemy->GetMatrix();
	}
}

void Enemy_Main::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}

void Enemy_Main::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Enemy_Main::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
