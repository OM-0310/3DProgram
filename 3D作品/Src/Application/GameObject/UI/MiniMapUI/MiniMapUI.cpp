#include "MiniMapUI.h"

void MiniMapUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/MiniMap/MiniMapUI/MiniMapUI.png");

	m_rect		= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos		= {};

	m_active	= false;
}

void MiniMapUI::DrawSprite()
{
	if (!m_active)return;
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect);
}
