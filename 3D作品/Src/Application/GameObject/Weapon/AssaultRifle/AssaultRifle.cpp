#include "AssaultRifle.h"

void AssaultRifle::Init()
{
	WeaponBase::SetAsset("Asset/Models/Weapon/AssaultRifle/AssaultRifle_2.gltf");
	
	m_localPos = m_unholdBasePoint;
	m_mScale = Math::Matrix::CreateScale(0.18f);
}

void AssaultRifle::Update()
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
