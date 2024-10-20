#include "CardKeyUI.h"

#include "../../CardKey/CardKey.h"

void CardKeyUI::Init()
{
	if (!m_spTex)
	{
		m_spTex = std::make_shared<KdTexture>();
		m_spTex->Load("Asset/Textures/UI/CardKeyUI/CardKeyUI.png");
	}

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos	= { 0.f, -220.f };
	m_alpha = 0.f;

	m_color = { 1.f,1.f,1.f,m_alpha };
}

void CardKeyUI::Update()
{
	std::shared_ptr<CardKey> spCard = m_wpCard.lock();

	if (spCard)
	{
		if (spCard->IsExpired())
		{
			m_isExpired = true;
		}
		if (spCard->GetCollectFlg())
		{
			m_alpha += 0.1f;
		}
		else
		{
			m_alpha -= 0.1f;
		}

		if (m_alpha >= 1.f)
		{
			m_alpha = 1.f;
		}
		if (m_alpha <= 0.f)
		{
			m_alpha = 0.f;
		}
	}
	m_color = { 1.f, 1.f, 1.f, m_alpha };
}

void CardKeyUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
