#include "SelectTitle.h"

void SelectTitle::Init()
{
	if (!m_spTex)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/GameOver/SelectTitle/SelectTitle.png");
		m_spTex = std::make_shared<KdTexture>();
		m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/GameOver/SelectTitle/SelectTitle.png");
	}

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 320.0f,-180.0f };

	m_alpha = m_alphaMax;

	m_color = { 1.0f,1.0f,1.0f,m_alpha };

	m_bitsEachFlg[ChoiceFlg] = false;
}

void SelectTitle::Update()
{
	if (m_bitsEachFlg[ChoiceFlg])
	{
		m_alpha = m_alphaMax;
	}
	else
	{
		m_alpha = m_alphaHalf;
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void SelectTitle::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
