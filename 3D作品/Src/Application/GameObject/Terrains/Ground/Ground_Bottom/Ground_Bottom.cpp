#include "Ground_Bottom.h"

void Ground_Bottom::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Ground/Ground_Bottom/Ground_Bottom.gltf");

	m_pos = { 0.f,-1.f,0.f };

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Ground_BottomCollision", m_spModel, KdCollider::TypeGround);
}

void Ground_Bottom::DrawLit()
{
	TerrainBase::DrawLit();
}
