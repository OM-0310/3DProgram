#include "Building.h"

void Building::Init()
{
	m_spModel	= std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Terrains/Building/Building.gltf");

	m_pos		= {0.0f,-1.f,0.0f};

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Ground", m_spModel, KdCollider::TypeGround);
}

void Building::DrawLit()
{
	TerrainBase::DrawLit();
}

void Building::GenerateDepthMapFromLight()
{
	//TerrainBase::GenerateDepthMapFromLight();
}
