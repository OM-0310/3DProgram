#include "VegetableBlock.h"

void VegetableBlock::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/WoodenBox/Wooden_Box.gltf");
	}

	m_pos	= { -5.f,0.1f,5.f };

	// 後で消す
	m_color = kGreenColor;

	m_fType = FoodType::Meet;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Vegetable", m_spModel, KdCollider::TypeGround);
}

void VegetableBlock::Update()
{
	m_mWorld = Math::Matrix::CreateTranslation(m_pos);
}
