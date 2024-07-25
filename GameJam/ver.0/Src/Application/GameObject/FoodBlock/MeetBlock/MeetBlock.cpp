#include "MeetBlock.h"

void MeetBlock::Init()
{
	m_spModel = std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/WoodenBox/Wooden_Box.gltf");

	m_pos = { 5.f,0.1f,5.f };

	// 後で消す
	m_color = kRedColor;

	m_fType = FoodType::Meet;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Meet", m_spModel, KdCollider::TypeGround);
}

void MeetBlock::Update()
{
	m_mWorld = Math::Matrix::CreateTranslation(m_pos);
}
