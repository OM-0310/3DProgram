#include "NoAnswerUI.h"

void NoAnswerUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/Answer/NoAnswerUI/NoAnswerUI.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 0.0f,-320.0f };

	m_lifeSpan = m_lifeSpanMax;
}

void NoAnswerUI::Update()
{
	m_lifeSpan--;
	if (m_lifeSpan <= m_lifeSpanMin)
	{
		m_isExpired = true;
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void NoAnswerUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
