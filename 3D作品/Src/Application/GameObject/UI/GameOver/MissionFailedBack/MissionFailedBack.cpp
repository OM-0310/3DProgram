#include "MissionFailedBack.h"

void MissionFailedUIBack::Init()
{
	UIBase::SetAsset("Asset/Textures/GameOver/MissionFailedBack/MissionFailedBack.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 0.0f, 0.0f };
}

void MissionFailedUIBack::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect);
}
