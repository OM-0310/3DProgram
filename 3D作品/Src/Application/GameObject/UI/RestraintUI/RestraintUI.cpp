#include "RestraintUI.h"

#include "../../Characters/Player/Player.h"
#include "../../Characters/Enemy/Enemy_1/Enemy_1.h"
#include "../../Characters/Enemy/Enemy_2/Enemy_2.h"
#include "../../Characters/Enemy/Enemy_3/Enemy_3.h"

#include "../../../Scene/SceneManager.h"

void RestraintUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/RestraintUI/RestraintUI.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { 0.f, -220.f };
	m_alpha			= 0.f;
	m_alphaSpeed	= 0.2f;

	m_restraintFlg	= false;
	m_able1Flg		= false;
	m_able2Flg		= false;
	m_able3Flg		= false;

	m_lifeSpan		= m_lifeSpanMax;

	m_color			= { 1.f,1.f,1.f,m_alpha };

	m_alphaState	= AlphaStateType::Dec;
}

void RestraintUI::Update()
{
	const std::shared_ptr<Player>	spPlayer = m_wpPlayer.lock();
	const std::shared_ptr<Enemy_1>	spEnemy_1 = m_wpEnemy_1.lock();
	const std::shared_ptr<Enemy_2>	spEnemy_2 = m_wpEnemy_2.lock();
	const std::shared_ptr<Enemy_3>	spEnemy_3 = m_wpEnemy_3.lock();
	Math::Vector3 diffVec1, diffVec2, diffVec3;

	if (spPlayer && spEnemy_1)
	{
		diffVec1 = spEnemy_1->GetPos() - spPlayer->GetPos();

		if (diffVec1.Length() <= spPlayer->GetTightArea())
		{
			if (!m_able1Flg)
			{
				m_able1Flg = true;
			}
		}
		else
		{
			if (m_able1Flg)
			{
				m_able1Flg = false;
				m_lifeSpan = m_lifeSpanMax;
			}
		}
	}

	if (spPlayer && spEnemy_2)
	{
		diffVec2 = spEnemy_2->GetPos() - spPlayer->GetPos();

		if (diffVec2.Length() <= spPlayer->GetTightArea())
		{
			if (!m_able2Flg)
			{
				m_able2Flg = true;
			}
		}
		else
		{
			if (m_able2Flg)
			{
				m_able2Flg = false;
				m_lifeSpan = m_lifeSpanMax;
			}
		}
	}

	if (spPlayer && spEnemy_3)
	{
		diffVec3 = spEnemy_3->GetPos() - spPlayer->GetPos();

		if (diffVec3.Length() <= spPlayer->GetTightArea())
		{
			if (!m_able3Flg)
			{
				m_able3Flg = true;
			}
		}
		else
		{
			if (m_able3Flg)
			{
				m_able3Flg = false;
				m_lifeSpan = m_lifeSpanMax;
			}
		}
	}

	if (!m_restraintFlg)
	{
		if (m_able1Flg || m_able2Flg || m_able3Flg)
		{
			m_alpha += m_alphaSpeed;
		}
		else if(!m_able1Flg && !m_able2Flg && !m_able3Flg)
		{
			m_alpha -= m_alphaSpeed;
		}
	}

	if (m_restraintFlg)
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
	}

	if (m_alpha >= m_alphaMax)
	{
		m_alpha = m_alphaMax;
	}
	if (m_alpha <= m_alphaMin)
	{
		m_alpha = m_alphaMin;
	}

	if (m_lifeSpan <= m_lifeSpanMin)
	{
		m_lifeSpan = m_lifeSpanMin;
		m_alpha = m_alphaMin;
	}

	m_color = { 1.f, 1.f, 1.f, m_alpha };
}

void RestraintUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}

void RestraintUI::SetRestraintFlg(const bool _restraintFlg)
{
	m_restraintFlg	= _restraintFlg;
}
