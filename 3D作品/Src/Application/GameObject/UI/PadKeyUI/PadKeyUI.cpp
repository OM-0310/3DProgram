#include "PadKeyUI.h"
#include "../../Gimmicks/LockedDoor/LockedDoor.h"

void PadKeyUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/PadKeyUI/PadKeyUI.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { 0.f, -220.f };
	m_alpha			= 0.f;
	m_alphaSpeed	= 0.1f;

	m_useFlg		= false;
	m_lifeSpan		= 70;
	m_animCnt		= 0;
	m_singleX		= m_spTex->GetWidth() / m_animMax;

	m_color			= { 1.f,1.f,1.f,m_alpha };

	m_alphaValue	= AlphaValue::Dec;
}

void PadKeyUI::Update()
{

	std::shared_ptr<LockedDoor> spLockedDoor = m_wpLockedDoor.lock();

	if (!m_useFlg)
	{
		if (spLockedDoor)
		{
			if (spLockedDoor->GetLockFlg())
			{
				m_useFlg = true;
			}
			if (spLockedDoor->GetOpeAbleFlg())
			{
				m_alpha += m_alphaSpeed;
			}
			else
			{
				m_alpha -= m_alphaSpeed;
			}

			if (m_alpha >= 1.f)
			{
				m_alpha = 1.f;
			}
			if (m_alpha <= 0.f)
			{
				m_alpha = 0.f;
			}
		}
	}
	else
	{
		m_lifeSpan--;
		m_animCnt += 0.5f;
		switch (m_alphaValue)
		{
		case AlphaValue::Inc:
			m_alpha += m_alphaSpeed;
			if (m_alpha >= 1.f)
			{
				m_alphaValue = AlphaValue::Dec;
				m_alpha = 1.f;
			}
			break;
		case AlphaValue::Dec:
			m_alpha -= m_alphaSpeed;
			if (m_alpha <= 0.f)
			{
				m_alphaValue = AlphaValue::Inc;
				m_alpha = 0.f;
			}
			break;
		}
		if (m_animCnt >= m_animMax)
		{
			m_animCnt = static_cast<float>(m_animMax) - 1;
		}
		if (m_lifeSpan == 0)
		{
			m_isExpired = true;
		}
	}

	m_color = { 1.f, 1.f, 1.f, m_alpha };
	m_rect = { m_singleX * static_cast<int>(m_animCnt),0,m_singleX,static_cast<long>(m_spTex->GetHeight()) };
}

void PadKeyUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
