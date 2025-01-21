#include "CardKeyUI.h"

#include "../../Item/CardKey/CardKey.h"

void CardKeyUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/CardKey/CardKeyUI/CardKeyUI.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { 0.f, -220.f };
	m_alpha			= 0.f;
	m_alphaSpeed	= 0.1f;

	m_useFlg		= false;
	m_lifeSpan		= 70;

	m_color			= { 1.f,1.f,1.f,m_alpha };

	m_alphaState	= AlphaStateType::Dec;
}

void CardKeyUI::Update()
{
	const std::shared_ptr<CardKey> spCard = m_wpCard.lock();

	if (!m_useFlg)
	{
		if (spCard)
		{
			if (spCard->IsExpired())
			{
				m_useFlg = true;
			}
			if (spCard->GetCollectFlg())
			{
				m_alpha += m_alphaSpeed;
			}
			else
			{
				m_alpha -= m_alphaSpeed;
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
	}
	else
	{
		m_lifeSpan--;
		switch (m_alphaState)
		{
		case AlphaStateType::Inc:
			m_alpha += m_alphaSpeed;
			if (m_alpha >= 1.f)
			{
				m_alphaState = AlphaStateType::Dec;
				m_alpha = 1.f;
			}
			break;
		case AlphaStateType::Dec:
			m_alpha -= m_alphaSpeed;
			if (m_alpha <= 0.f)
			{
				m_alphaState = AlphaStateType::Inc;
				m_alpha = 0.f;
			}
			break;
		}
		if (m_lifeSpan == 0)
		{
			m_isExpired = true;
		}
	}
	m_color = { 1.f, 1.f, 1.f, m_alpha };

}

void CardKeyUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
