#include "CardKeyUI.h"

#include "../../Item/CardKey/CardKey.h"

void CardKeyUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/CardKey/CardKeyUI/CardKeyUI.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= { 0.f, -220.f };
	m_alpha			= 0.f;
	m_alphaSpeed	= 0.2f;

	m_useFlg		= false;
	m_lifeSpan		= m_lifeSpanMax;

	m_color			= { 1.f,1.f,1.f,m_alpha };

	m_alphaState	= AlphaStateType::Dec;
}

void CardKeyUI::Update()
{
	const std::shared_ptr<CardKey> spCard = m_wpCard.lock();

	// 使用フラグがfalseのとき
	if (!m_useFlg)
	{
		// カードキーの情報があるとき
		if (spCard)
		{
			// カードキーのクラスが破棄されたとき
			if (spCard->IsExpired())
			{
				// 使用フラグをtrueにする
				m_useFlg = true;
			}

			// カードキーのアイテム回収フラグがtrueのとき
			if (spCard->GetCollectFlg())
			{
				// アルファ値を加算
				m_alpha += m_alphaSpeed;
			}
			// falseのとき
			else
			{
				// アルファ値を減算
				m_alpha -= m_alphaSpeed;
			}

			// アルファ値が最大値以上のとき
			if (m_alpha >= m_alphaMax)
			{
				// アルファ値を最大値に設定
				m_alpha = m_alphaMax;
			}

			// アルファ値が最小値以下のとき
			if (m_alpha <= m_alphaMin)
			{
				// アルファ値を最小値に設定
				m_alpha = m_alphaMin;
			}
		}
	}
	else
	{
		// 生存期間を減少
		m_lifeSpan--;

		switch (m_alphaState)
		{
		case AlphaStateType::Inc:

			// アルファ値を加算
			m_alpha += m_alphaSpeed;

			// アルファ値が最大値以上のとき
			if (m_alpha >= m_alphaMax)
			{
				// アルファ値の加減を"減少状態"に切り替え
				m_alphaState = AlphaStateType::Dec;

				// アルファ値を最大値に設定
				m_alpha = m_alphaMax;
			}
			break;
		case AlphaStateType::Dec:

			// アルファ値を減算
			m_alpha -= m_alphaSpeed;

			// アルファ値が最小値以下のとき
			if (m_alpha <= m_alphaMin)
			{
				// アルファ値の加減を"増加状態"に切り替え
				m_alphaState = AlphaStateType::Inc;

				// アルファ値を最小値に設定
				m_alpha = m_alphaMin;
			}
			break;
		}

		// 生存期間が最小値以下のとき
		if (m_lifeSpan <= m_lifeSpanMin)
		{
			// クラスを破棄する
			m_isExpired = true;
		}
	}

	// 色情報を確定
	m_color = { 1.f, 1.f, 1.f, m_alpha };
}

void CardKeyUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
