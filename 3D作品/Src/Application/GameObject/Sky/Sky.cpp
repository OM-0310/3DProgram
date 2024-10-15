#include "Sky.h"

void Sky::Init()
{
	m_spModel = std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Sky/sky.gltf");

	m_pos = { 0.0f,0.0f,0.0f };

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mScale = Math::Matrix::CreateScale(100.0f);
	m_mWorld = m_mScale * m_mTrans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Sky", m_spModel, KdCollider::TypeDamage);
}

void Sky::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
