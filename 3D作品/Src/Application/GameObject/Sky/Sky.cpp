#include "Sky.h"

void Sky::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Sky/Sky.gltf");
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Sky/Sky.gltf"));
	}

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
