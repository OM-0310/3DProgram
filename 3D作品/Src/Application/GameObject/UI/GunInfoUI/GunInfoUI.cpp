#include "GunInfoUI.h"

void GunInfoUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/GunInfoUI/GunInfoUI.png");

	m_rect	= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 530.0f,-300.0f };
}

void GunInfoUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect);
}
