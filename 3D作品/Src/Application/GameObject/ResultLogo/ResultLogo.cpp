#include "ResultLogo.h"

void ResultLogo::Init()
{
	if (!m_spTex)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/ResultLogo/ResultLogo.png");
		m_spTex = std::make_shared<KdTexture>();
		m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/ResultLogo/ResultLogo.png");
	}

	m_pos = { 0.0f,0.0f };
}

void ResultLogo::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, (int)m_pos.x, (int)m_pos.y);
}
