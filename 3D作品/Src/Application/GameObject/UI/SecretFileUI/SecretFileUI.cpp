#include "SecretFileUI.h"

#include "../../../GameObject/Item/SecretFile/SecretFile.h"

void SecretFileUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/SecretFile/SecretFileUI/SecretFileUI.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { 0.f, -220.f };
	m_alpha			= m_alphaMin;
	m_alphaSpeed	= 0.2f;

	m_useFlg		= false;
	m_lifeSpan		= m_lifeSpanMax;

	m_color			= { 1.f,1.f,1.f,m_alpha };

	m_alphaState	= AlphaStateType::Dec;
}

void SecretFileUI::Update()
{
	std::shared_ptr<SecretFile> spFile = m_wpFile.lock();

	if (!m_useFlg)
	{
		if (spFile)
		{
			if (spFile->IsExpired())
			{
				m_useFlg = true;
			}
			if (spFile->GetCollectFlg())
			{
				m_alpha += m_alphaSpeed;
			}
			else
			{
				m_alpha -= m_alphaSpeed;
			}

			if (m_alpha >= m_alphaMax)
			{
				m_alpha = m_alphaMax;
			}
			if (m_alpha <= m_alphaMin)
			{
				m_alpha = m_alphaMin;
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
			if (m_alpha >= m_alphaMax)
			{
				m_alphaState = AlphaStateType::Dec;
				m_alpha = m_alphaMax;
			}
			break;
		case AlphaStateType::Dec:
			m_alpha -= m_alphaSpeed;
			if (m_alpha <= m_lifeSpanMin)
			{
				m_alphaState = AlphaStateType::Inc;
				m_alpha = m_alphaMin;
			}
			break;
		}
		if (m_lifeSpan <= m_lifeSpanMin)
		{
			m_isExpired = true;
		}
	}
	m_color = { 1.f, 1.f, 1.f, m_alpha };
}

void SecretFileUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
