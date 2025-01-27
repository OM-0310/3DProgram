#include "Game_Back.h"

#include "../Characters/Player/Player.h"

void Game_Back::Init()
{
	if (!m_spTex)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/BlackWindow/BlackWindow.png");
		m_spTex = std::make_shared<KdTexture>();
		m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/BlackWindow/BlackWindow.png");
	}

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 0.0f,0.0f };

	m_alpha = m_alphaMax;

	m_color = { 1.0f,1.0f,1.0f,m_alpha };

	m_feedOut = false;
}

void Game_Back::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		if (spPlayer->GetClearFlg())
		{
			m_feedOut = true;
		}
	}

	if (m_feedOut)
	{
		m_alpha += m_alphaSpd;

		if (m_alpha >= m_alphaMax)
		{
			m_alpha = m_alphaMax;
		}
	}
	else 
	{
		m_alpha -= m_alphaSpd;

		if(m_alpha <= m_alphaMin)
		{
			m_alpha = m_alphaMin;
		}
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void Game_Back::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
