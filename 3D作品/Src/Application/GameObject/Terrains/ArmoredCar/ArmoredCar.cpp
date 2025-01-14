#include "ArmoredCar.h"

void ArmoredCar::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/ArmoredCar/ArmoredCar.gltf");

	m_pos = { 0.f,-1.f,0.f };

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("ArmoredCarCollision", m_spModel, KdCollider::TypeGround | KdCollider::TypeBump);
}

void ArmoredCar::DrawLit()
{
	TerrainBase::DrawLit();
}

void ArmoredCar::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
