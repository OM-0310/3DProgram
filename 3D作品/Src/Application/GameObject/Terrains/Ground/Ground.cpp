#include "Ground.h"

void Ground::Init()
{
	m_spModel	= std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Terrains/Ground/Ground.gltf");

	m_pos		= { 0.f,-1.f,10.f };

	m_mScale	= Math::Matrix::CreateScale(100.0f);
	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld	= m_mScale * m_mTrans;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Ground", m_spModel, KdCollider::TypeGround);
}

void Ground::DrawLit()
{
	TerrainBase::DrawLit();
}
