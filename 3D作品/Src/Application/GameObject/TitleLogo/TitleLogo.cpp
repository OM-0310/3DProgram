#include "TitleLogo.h"

void TitleLogo::Init()
{
	if (!m_spTex)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/TitleLogo/TitleLogo.png");
		m_spTex = std::make_shared<KdTexture>();
		m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/TitleLogo/TitleLogo.png");
	}

	m_pos = { 0.0f,180.0f };
}

void TitleLogo::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, (int)m_pos.x, (int)m_pos.y);
}
