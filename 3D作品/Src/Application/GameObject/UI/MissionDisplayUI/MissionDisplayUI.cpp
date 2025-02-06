#include "MissionDisplayUI.h"

void MissionDisplayUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/MissionDisplayUI/MissionDisplayUI.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 0.0f, 200.0f };

	m_alpha = m_alphaMin;

	m_color = { 1.0f,1.0f,1.0f,m_alpha };

	m_alphaSpeed = 0.05f;

	m_lifeSpan = m_lifeSpanMin;

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}
}

void MissionDisplayUI::Update()
{
	if (!m_bitsEachFlg.test(FeedOutFlg))
	{
		m_alpha += m_alphaSpeed;

		if (m_alpha >= m_alphaMax)
		{
			m_alpha = m_alphaMax;

			if (!m_bitsEachFlg.test(lifeDecFlg))
			{
				m_bitsEachFlg.set(lifeDecFlg, true);
				m_lifeSpan = m_lifeSpanMax;
			}
		}
	}
	else
	{
		m_alpha -= m_alphaSpeed;

		if (m_alpha <= m_alphaMin)
		{
			m_alpha = m_alphaMin;
			m_isExpired = true;
		}
	}

	if (m_bitsEachFlg.test(lifeDecFlg))
	{
		m_lifeSpan--;
		if (m_lifeSpan <= m_lifeSpanMin)
		{
			m_lifeSpan = m_lifeSpanMin;
			m_bitsEachFlg.set(FeedOutFlg, true);
		}
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void MissionDisplayUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
