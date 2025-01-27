#include "SubMissionUI.h"

void SubMissionUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/SubMissionUI/SubMissionUI.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { -390.0f,65.0f };

	m_permissionFlg = false;
	m_active		= false;
}

void SubMissionUI::DrawSprite()
{
	if (m_permissionFlg)
	{
		if (!m_active)return;
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect);
	}
}
