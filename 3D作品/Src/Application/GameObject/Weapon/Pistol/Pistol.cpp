#include "Pistol.h"

void Pistol::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/Weapon/Pistol/Pistol_1.gltf");
	}

	m_localPos	= m_unholdBasePoint;
//	m_mRot		= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));
	m_mScale	= Math::Matrix::CreateScale(0.18f);
}

void Pistol::Update()
{
	if (m_holdFlg)
	{
		m_localPos = m_holdBasePoint;
	}
	else
	{
		m_localPos = m_unholdBasePoint;
	}

	m_mLocal = Math::Matrix::CreateTranslation(m_localPos);

	WeaponBase::Update();
}
