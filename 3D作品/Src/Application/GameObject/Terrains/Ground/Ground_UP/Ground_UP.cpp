#include "Ground_UP.h"

void Ground_UP::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Ground/Ground_UP/Ground_UP.gltf");

	m_pos = { 0.f,-1.f,0.f };

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Ground_BottomCollision", m_spModel, KdCollider::TypeGround | KdCollider::TypeBump);
}

void Ground_UP::DrawLit()
{
	TerrainBase::DrawLit();
}
