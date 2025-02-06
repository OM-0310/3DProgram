#include "MiniMapUIBack.h"

void MiniMapUIBack::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/MiniMap/MiniMapUIBack/MiniMapUIBack.png");

	m_rect		= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos		= {};

	m_alpha		= 0.5f;

	m_color		= { 1.0f,1.0f,1.0f,m_alpha };

	m_bitsEachFlg.reset();
}

void MiniMapUIBack::Update()
{
	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void MiniMapUIBack::DrawSprite()
{
	if (!m_bitsEachFlg.test(ActiveFlg))return;
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
