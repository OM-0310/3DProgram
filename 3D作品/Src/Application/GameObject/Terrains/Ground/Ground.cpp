#include "Ground.h"

void Ground::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Ground/Ground.gltf");

	m_pos		= { 0.f,-1.f,10.f };

	m_mScale	= Math::Matrix::CreateScale(5.0f);
	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld	= m_mScale * m_mTrans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("GroundCollision", m_spModel, KdCollider::TypeGround);
}

void Ground::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.SetUVTiling({ 50,50 });
	TerrainBase::DrawLit();
}
