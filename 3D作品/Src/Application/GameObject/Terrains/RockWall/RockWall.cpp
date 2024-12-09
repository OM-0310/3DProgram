#include "RockWall.h"

void RockWall::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/RockWall/RockWall.gltf");

	m_pos = { 0.f,-1.f,0.f };

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("RockWallCollision", m_spModel, KdCollider::TypeGround);
}

void RockWall::DrawLit()
{
	TerrainBase::DrawLit();
}

void RockWall::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
