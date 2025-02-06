#include "Game_Back.h"

#include "../UI/MissionDisplayUI/MissionDisplayUI.h"
#include "../Characters/Player/Player.h"

#include "../../Scene/SceneManager.h"

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

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}
}

void Game_Back::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		if (spPlayer->GetFeedOutFlg())
		{
			m_bitsEachFlg.set(FeedOutFlg, true);
		}
	}

	if (m_bitsEachFlg.test(FeedOutFlg))
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

			if (!m_bitsEachFlg.test(DisplayFlg))
			{
				m_bitsEachFlg.set(DisplayFlg, true);

				std::shared_ptr<MissionDisplayUI> displayUI;
				displayUI = std::make_shared<MissionDisplayUI>();
				displayUI->Init();
				SceneManager::Instance().AddObject(displayUI);
			}
		}
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void Game_Back::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
