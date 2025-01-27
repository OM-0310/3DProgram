#include "NeedCradKeyUI.h"

void NeedCardKeyUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/NeedCardKeyUI/NeedCardKeyUI.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = { 784.0f, 220.0f };

	m_actionCnt = 0;

	m_progressState = ProgressStateType::Progress;
}

void NeedCardKeyUI::Update()
{
	switch (m_progressState)
	{
	case ProgressStateType::Stop:

		// アクションカウントを増やす
		m_actionCnt++;

		// アクションカウントが最大値以上のとき
		if (m_actionCnt >= m_actionCntMax)
		{
			// 進行状態を後退状態にする
			m_progressState = ProgressStateType::Retreat;
		}

		break;
	case ProgressStateType::Progress:

		// 現在のX座標を減算する
		m_pos.x -= m_moveSpeed;

		// 現在のX座標が中間地点以下のとき
		if (m_pos.x <= m_middlePosX)
		{
			// X座標を中間地点に設定
			m_pos.x = m_middlePosX;

			// 進行状態を停止状態にする
			m_progressState = ProgressStateType::Stop;
		}

		break;
	case ProgressStateType::Retreat:

		// 現在のX座標を加算する
		m_pos.x += m_moveSpeed;

		// 現在のX座標が到達地点以上のとき
		if (m_pos.x >= m_arrivalPosX)
		{
			// X座標を到達地点に設定
			m_pos.x = m_arrivalPosX;

			// クラスを破棄する
			m_isExpired = true;
		}
		break;
	}
}

void NeedCardKeyUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect);
}
