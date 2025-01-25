#include "AnswerUI.h"

#include "../CardKeyLocation/CardKeyLocation.h"
#include "../SecretFileLocation/SecretFileLocation.h"

void AnswerUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/Answer/AnswerUI/AnswerUI.png");

	m_rect		= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos		= { 0.0f,-320.0f };

	m_lifeSpan	= m_lifeSpanMax;
}

void AnswerUI::Update()
{
	const std::shared_ptr<CardKeyLocation>		spCardLocation = m_wpCardLocation.lock();
	const std::shared_ptr<SecretFileLocation>	spFileLocation = m_wpFileLocation.lock();

	m_lifeSpan--;
	if (m_lifeSpan <= m_lifeSpanMin)
	{
		if (spFileLocation)
		{
			if (!spFileLocation->GetPermissionFlg())
			{
				spFileLocation->Consent(true);
			}
			else
			{
				if (spCardLocation)
				{
					spCardLocation->Consent(true);
				}
			}
		}

		m_isExpired = true;
	}
}

void AnswerUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
