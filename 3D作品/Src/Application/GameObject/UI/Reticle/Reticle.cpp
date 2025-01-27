#include "Reticle.h"

#include "../../Camera/TPSCamera/TPSCamera.h"

void Reticle::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/Reticle/Reticle.png");

	m_pos = m_aimRPos;

	m_color = kWhiteColor;

	m_Active = false;
}

void Reticle::Update()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpTPSCamera.lock();

	if (_spCamera)
	{
		if (_spCamera->GetCamType() == TPSCamera::CameraType::AimR)
		{
			m_pos = m_aimRPos;
		}
		else if(_spCamera->GetCamType() == TPSCamera::CameraType::AimL)
		{
			m_pos = m_aimLPos;
		}
	}
}

void Reticle::DrawSprite()
{
	if (!m_Active)return;

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, (int)m_pos.x, (int)m_pos.y);
}
