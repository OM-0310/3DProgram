#include "HPBarUIBack.h"

void HPBarUIBack::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/HPBar/HPBarUIBack/HPBarUIBack.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pivot = { 0.0f,0.5f };

	m_pos = { -600.0f,-330.0f };
}

void HPBarUIBack::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color, m_pivot);
}
