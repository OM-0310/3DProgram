#include "Bed.h"

void Bed::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Building/Furniture/Bed/Bed.gltf");

	m_mRot		= Math::Matrix::Identity;
	m_mTrans	= Math::Matrix::Identity;

	m_pos		= { 0.0f,-1.0f,0.0f };

	m_angle		= 90.0f;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("BedCollision", m_spModel, KdCollider::TypeGround);
}

void Bed::Update()
{
	m_mRot	 = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;
}
