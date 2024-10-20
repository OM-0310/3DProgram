#include "Pistol_Disarm.h"

#include "../../../Characters/Player/Player.h"

void Pistol_Disarm::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/Weapon/Pistol/Pistol_Disarm.gltf");

		m_alpha = 1.0f;
		m_color = { 1.f,1.f,1.f,m_alpha };
	}
}

void Pistol_Disarm::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	if (spPlayer)
	{
		Math::Matrix rotMat;
		rotMat = spPlayer->GetRotateMat();

		Math::Matrix transMat;
		m_disarmPos = spPlayer->GetDisarmPos();
		transMat = Math::Matrix::CreateTranslation(m_disarmPos);

		m_mWorld = rotMat * transMat;

		if (spPlayer->GetHoldFlg())
		{
			m_alpha = 0.f;
		}
		else
		{
			m_alpha = 1.f;
		}
		m_color = { 1.f,1.f,1.f,m_alpha };
	}
	else
	{
		assert("spPlayer 情報がありません");
	}
}
