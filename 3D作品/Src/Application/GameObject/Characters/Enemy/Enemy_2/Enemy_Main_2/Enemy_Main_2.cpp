﻿#include "Enemy_Main_2.h"

#include "../Enemy_2.h"

#include "../../../../../main.h"

void Enemy_Main_2::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Enemy/Enemy_Main/Enemy_Main.gltf");
	m_spAnimator	= std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_alpha			= 1.0f;

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}

	m_pos			= { 0.f,0.f,1.0f };

	m_HP			= MAXHP;

	m_objectType	= ObjectType::TypeEnemy_2;

	m_pCollider		= std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Enemy_Main_2Collision", m_spModel, KdCollider::TypeDamage);
}

void Enemy_Main_2::Update()
{
	const std::shared_ptr<Enemy_2> spEnemy = m_wpEnemy.lock();

	if (spEnemy)
	{
		m_mWorld = spEnemy->GetMatrix();

		if (m_HP <= 0)
		{
			spEnemy->SetDeathFlg(true);
		}

		if (spEnemy->GetDissolveFlg())
		{
			if (!m_bitsEachFlg.test(FeedOutFlg))
			{
				m_bitsEachFlg.set(FeedOutFlg, true);
			}
		}
	}

	if (m_bitsEachFlg.test(FeedOutFlg))
	{
		m_alpha -= m_alphaSpeed;
	}

	if (m_alpha <= m_alphaMin)
	{
		m_alpha = m_alphaMin;
		m_isExpired = true;
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void Enemy_Main_2::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}

void Enemy_Main_2::DrawLit()
{
	if (!m_spModel)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void Enemy_Main_2::GenerateDepthMapFromLight()
{
	if (!m_spModel)return;

	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}
