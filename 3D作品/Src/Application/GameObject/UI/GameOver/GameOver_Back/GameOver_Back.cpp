#include "GameOver_Back.h"

#include "../SelectTitle/SelectTitle.h"
#include "../SelectRetry/SelectRetry.h"

void GameOver_Back::Init()
{
	if (!m_spTex)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/BlackWindow/BlackWindow.png");
		m_spTex = std::make_shared<KdTexture>();
		m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/BlackWindow/BlackWindow.png");
	}

	m_spSelectSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/GameOver/Select.wav");
	m_spSelectSound->SetVolume(m_selectVol);

	m_spPressSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/GameOver/PressButton.wav");
	m_spPressSound->SetVolume(m_pressVol);

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { 0.0f,0.0f };
	
	m_alpha			= m_alphaMax;

	m_color			= { 1.0f,1.0f,1.0f,m_alpha };

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}
}

void GameOver_Back::Update()
{
	const std::shared_ptr<SelectTitle> spTitle = m_wpTitle.lock();
	const std::shared_ptr<SelectRetry> spRetry = m_wpRetry.lock();

	if (GetAsyncKeyState('A') & 0x8000 && !m_bitsEachFlg.test(AKeyFlg))
	{
		m_bitsEachFlg.set(AKeyFlg, true);

		m_spSelectSound->Play();

		if (spTitle && spRetry)
		{
			if (!spTitle->GetChoiceFlg() && spRetry->GetChoiceFlg())
			{
				spTitle->SetChoiseFlg(true);
				spRetry->SetChoiseFlg(false);
			}
			else if (spTitle->GetChoiceFlg() && !spRetry->GetChoiceFlg())
			{
				spTitle->SetChoiseFlg(false);
				spRetry->SetChoiseFlg(true);
			}
		}

	}
	else if (!(GetAsyncKeyState('A') & 0x8000))
	{
		m_bitsEachFlg.set(AKeyFlg, false);
	}

	if (GetAsyncKeyState('D') & 0x8000 && !m_bitsEachFlg.test(DKeyFlg))
	{
		m_bitsEachFlg.set(DKeyFlg, true);

		m_spSelectSound->Play();

		if (spTitle && spRetry)
		{
			if (!spTitle->GetChoiceFlg() && spRetry->GetChoiceFlg())
			{
				spTitle->SetChoiseFlg(true);
				spRetry->SetChoiseFlg(false);
			}
			else if (spTitle->GetChoiceFlg() && !spRetry->GetChoiceFlg())
			{
				spTitle->SetChoiseFlg(false);
				spRetry->SetChoiseFlg(true);
			}
		}

	}
	else if (!(GetAsyncKeyState('D') & 0x8000))
	{
		m_bitsEachFlg.set(DKeyFlg, false);
	}

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!m_bitsEachFlg.test(EnterKeyFlg))
		{
			m_bitsEachFlg.set(EnterKeyFlg, true);
			m_bitsEachFlg.set(FeedOutFlg, true);

			if (m_spPressSound->IsStopped())
			{
				m_spPressSound->Play();
			}
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

		if (m_alpha <= m_alphaMin)
		{
			m_alpha = m_alphaMin;
		}
	}

	m_color = { 1.0f,1.0f,1.0f,m_alpha };
}

void GameOver_Back::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
