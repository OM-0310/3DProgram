#include "Building_Main.h"

void Building_Main::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Building/Bulding_Main/Building_Main.gltf");

	m_pos = { 0.f,-1.f,0.f };

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("BuildMainCollision", m_spModel, KdCollider::TypeGround);
}

void Building_Main::DrawLit()
{
	TerrainBase::DrawLit();
}
