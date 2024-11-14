#include "Pistol_Disarm.h"

#include "../../../Characters/Player/Player.h"

void Pistol_Disarm::Init()
{
	WeaponBase::SetAsset("Asset/Models/Weapon/Pistol/Pistol_Disarm.gltf");

	m_alpha		= 1.0f;
	m_color		= { 1.f,1.f,1.f,m_alpha };

	m_localPos	= {0.155f,1.f,0.1f};
	m_angle		= 0.f;

	m_mLocal	= Math::Matrix::CreateTranslation(m_localPos);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Pistol_Disarm::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	Math::Matrix			parentMat;

	if (spPlayer)
	{
		parentMat = spPlayer->GetMatrix();

		m_mWorld = m_mRot * m_mLocal * parentMat;

		if (spPlayer->GetPostureType() == Player::PostureType::Stand)
		{
		}

		if (spPlayer->GetHoldFlg())
		{
			m_alpha = 0.f;
		}
		else
		{
			m_alpha = 1.f;
		}
	}
	else
	{
		assert("spPlayer 情報がありません");
	}

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_mWorld.Translation();
	sphere.m_sphere.Radius = 0.05f;
	sphere.m_type = KdCollider::TypeDamage;

	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kBlueColor);

	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Pistol_Disarm::DrawLit()
{
	WeaponBase::DrawLit();
}

void Pistol_Disarm::GenerateDepthMapFromLight()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		if (!spPlayer->GetHoldFlg())
		{
			WeaponBase::GenerateDepthMapFromLight();
		}
	}
}
