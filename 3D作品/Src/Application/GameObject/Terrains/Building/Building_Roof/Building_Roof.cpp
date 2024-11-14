#include "Building_Roof.h"

void Building_Roof::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Building/Building_Roof/Building_Roof.gltf");

	m_pos = { 0.f,-1.f,0.f };

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("BuildRoofCollision", m_spModel, KdCollider::TypeGround);
}

void Building_Roof::DrawLit()
{
	TerrainBase::DrawLit();
}
