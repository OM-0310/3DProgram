#include "Title_Back.h"

void Title_Back::Init()
{
	if (!m_spTex)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/BlackWindow/BlackWindow.png");
		m_spTex = std::make_shared<KdTexture>();
		m_spTex = KdAssets::Instance().m_textures.GetData("Asset/Textures/BlackWindow/BlackWindow.png");
	}

	m_spSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/Title/PressEnter.wav");
	m_spSound->SetVolume(m_volume);

	m_rect		= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos		= { 0.0f,0.0f };

	m_alpha		= m_alphaMax;

	m_color		= { 1.0f,1.0f,1.0f,m_alpha };

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}
}

void Title_Back::Update()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (!m_bitsEachFlg.test(KeyFlg))
		{
			m_bitsEachFlg.set(KeyFlg, true);

			m_bitsEachFlg.set(FeedOutFlg, true);

			m_spSound->Play();
		}
	}
	else
	{
		m_bitsEachFlg.set(KeyFlg, false);
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

void Title_Back::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
