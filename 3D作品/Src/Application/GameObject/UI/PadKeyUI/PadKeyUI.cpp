#include "PadKeyUI.h"
#include "../../Gimmicks/LockedDoor/LockedDoor.h"

void PadKeyUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/PadKeyUI/PadKeyUI.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { 0.f, -220.f };
	m_alpha			= 0.f;
	m_alphaSpeed	= 0.2f;

	m_bitsEachFlg.reset();
	m_lifeSpan		= m_lifeSpanMax;
	m_animCnt		= 0;
	m_singleX		= static_cast<short>(m_spTex->GetWidth()) / static_cast<short>(m_animMax);

	m_color			= { 1.f,1.f,1.f,m_alpha };

	m_alphaState	= AlphaStateType::Dec;
}

void PadKeyUI::Update()
{

	const std::shared_ptr<LockedDoor> spLockedDoor = m_wpLockedDoor.lock();

	if (!m_bitsEachFlg.test(UseFlg))
	{
		if (spLockedDoor)
		{
			if (spLockedDoor->GetLockFlg())
			{
				m_bitsEachFlg.set(UseFlg, true);
			}
			if (spLockedDoor->GetOpeAbleFlg())
			{
				m_alpha += m_alphaSpeed;
			}
			else
			{
				m_alpha -= m_alphaSpeed;
			}

			if (m_alpha >= m_alphaMax)
			{
				m_alpha = m_alphaMax;
			}
			if (m_alpha <= m_alphaMin)
			{
				m_alpha = m_alphaMin;
			}
		}
	}
	else
	{
		m_lifeSpan--;
		m_animCnt += m_animSpeed;
		switch (m_alphaState)
		{
		case AlphaStateType::Inc:
			m_alpha += m_alphaSpeed;
			if (m_alpha >= m_alphaMax)
			{
				m_alphaState = AlphaStateType::Dec;
				m_alpha = m_alphaMax;
			}
			break;
		case AlphaStateType::Dec:
			m_alpha -= m_alphaSpeed;
			if (m_alpha <= m_alphaMin)
			{
				m_alphaState = AlphaStateType::Inc;
				m_alpha = m_alphaMin;
			}
			break;
		}
		if (m_animCnt >= m_animMax)
		{
			m_animCnt = static_cast<float>(m_animMax) - 1;
		}
		if (m_lifeSpan <= m_lifeSpanMin)
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
