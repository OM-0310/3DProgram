#include "HPBarUI.h"

#include "../../Characters/Player/Player_Main/Player_Main.h"

void HPBarUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/HPBarUI/HPBar.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pivot = { 0.0f,0.5f };

	m_pos = { -600.0f,-330.0f };

	m_color = { 0.0f, 1.0f, 0.0f, 1.0f };
}

void HPBarUI::Update()
{
	const std::shared_ptr<Player_Main> spPlayer_Main = m_wpPlayer_Main.lock();
	int hp = 0, width = 0, singleWidth = 0;

	// プレイヤーの上藩士の情報があるとき
	if (spPlayer_Main)
	{
		// プレイヤーのHPを取得
		hp = spPlayer_Main->GetHP();

		// 画像の横幅を取得
		width = m_spTex->GetWidth();

		// 横幅と最大HPを割り算しHP1分の横幅を算出
		singleWidth = width / spPlayer_Main->GetMaxHP();

		// 各パラメータを代入
		m_rect = { 0,0,(long)(singleWidth * hp),(long)m_spTex->GetHeight() };

		// HPが最大HP以下かつHP半分より大きいとき
		if (hp > spPlayer_Main->GetHalfHP() && hp <= spPlayer_Main->GetMaxHP())
		{
			// HPバーを緑色に表示する
			m_color = kGreenColor;
		}
		// HPが半分以下かつHP4分の1より大きいとき
		else if (hp > spPlayer_Main->GetQuaterHP() && hp <= spPlayer_Main->GetHalfHP())
		{
			m_color = { 1.0f,1.0f,0.0f,1.0f };
		}
		// HPが4分の1以下のとき
		else if (hp <= spPlayer_Main->GetQuaterHP())
		{
			m_color = kRedColor;
		}
	}
}

void HPBarUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color, m_pivot);
}
