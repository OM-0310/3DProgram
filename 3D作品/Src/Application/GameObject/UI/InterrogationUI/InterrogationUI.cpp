#include "InterrogationUI.h"

#include "../../Characters/Player/Player.h"

#include "../KillUI/KillUI.h"

void InterrogationUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/InterrogationUI/InterrogationUI.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { -110.f, -220.f };
	m_alpha			= m_alphaMin;
	m_alphaSpeed	= 0.2f;

	m_useFlg		= false;
	m_lifeSpan		= m_lifeSpanMax;

	m_color			= { 1.f,1.f,1.f,m_alpha };

	m_alphaState	= AlphaStateType::Dec;
}

void InterrogationUI::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	const std::shared_ptr<KillUI> spKillUI = m_wpKillUI.lock();

	if (spKillUI)
	{
		if (!m_useFlg)
		{
			if (spPlayer)
			{
				if (spPlayer->GetSituationType() & Player::SituationType::Restraint_Idle)
				{
					if (spKillUI)
					{
						if (spKillUI->GetUseFlg())
						{
							m_alpha -= m_alphaSpeed;
						}
						else
						{
							m_alpha += m_alphaSpeed;
						}
					}
				}
				else
				{
					m_alpha -= m_alphaSpeed;
				}
			}

			if (spKillUI)
			{
				if (spKillUI->GetUseFlg())
				{
					m_alpha -= m_alphaSpeed;
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
				if (m_alpha <= m_alphaMin)
				{
					m_alphaState = AlphaStateType::Inc;
					m_alpha = m_alphaMin;
				}
				break;
			}
			if (m_lifeSpan <= m_lifeSpanMin)
			{
				if (m_useFlg)
				{
					m_useFlg = false;
					m_lifeSpan = m_lifeSpanMax;
				}
			}
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

	m_color = { 1.0f, 1.0f, 1.0f, m_alpha };
}

void InterrogationUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
