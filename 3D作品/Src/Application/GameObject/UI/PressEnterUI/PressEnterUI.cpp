#include "PressEnterUI.h"

void PressEnterUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/PressEnterUI/PressEnterUI.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 0.0f,-200.0f };

	m_bitsEachFlg.reset();

	m_alpha = m_alphaMax;

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void PressEnterUI::Update()
{
	if (m_bitsEachFlg.test(FeedOutFlg))
	{
		m_alpha += m_alphaSpd;

		if (m_alpha >= m_alphaMax)
		{
			m_alpha = m_alphaMax;

			m_bitsEachFlg.set(FeedOutFlg, false);
		}
	}
	else
	{
		m_alpha -= m_alphaSpd;

		if (m_alpha <= m_alphaMin)
		{
			m_alpha = m_alphaMin;

			m_bitsEachFlg.set(FeedOutFlg, true);
		}
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void PressEnterUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
