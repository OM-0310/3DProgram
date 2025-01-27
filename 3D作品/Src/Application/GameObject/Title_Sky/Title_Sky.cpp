#include "Title_Sky.h"

void Title_Sky::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Sky/Sky.gltf");
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Sky/Sky.gltf"));
	}

	m_pos = { 0.0f,0.0f,0.0f };

	m_angleFlg = false;

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Sky", m_spModel, KdCollider::TypeDamage);
}

void Title_Sky::Update()
{
	if (!m_angleFlg)
	{
		m_angle -= m_angleSpd;
		if (m_angle <= -m_angleMax)
		{
			m_angle += m_angleMax;
		}
	}
	else
	{
		m_angle += m_angleSpd;
		if (m_angle >= m_angleMax)
		{
			m_angle -= m_angleMax;
		}
	}

	m_mScale	= Math::Matrix::CreateScale(10.0f);
	m_mRot		= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld	= m_mScale * m_mRot * m_mTrans;
}

void Title_Sky::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.SetUVTiling({ 5,5 });
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
