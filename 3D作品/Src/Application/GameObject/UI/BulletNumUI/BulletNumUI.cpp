﻿#include "BulletNumUI.h"

#include "../../Characters/Player/Player_Ready_Pistol/Player_Ready_Pistol.h"

void BulletNumUI::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/GunInfo/BulletNumUI/BulletNumUI.png");

	m_rect	= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pivot = { 1.0f,0.0f };

	m_pos	= { 640.0f,-360.0f };

}

void BulletNumUI::Update()
{
	std::shared_ptr<Player_Ready_Pistol> spPlayer_Ready_Pistol = m_wpPlayer_Ready_Pistol.lock();
	UINT	bulletNum	= 0; 
	int		singleWidth = 0;

	// 銃の情報があるとき
	if (spPlayer_Ready_Pistol)
	{
		// 残弾数を取得
		bulletNum = spPlayer_Ready_Pistol->GetNowBullet();

		// 残弾数1つ分の横幅を算出
		singleWidth = m_spTex->GetWidth() / spPlayer_Ready_Pistol->GetMagazinSize();
	}

	// 描画範囲の各パラメータに代入
	m_rect = {
		0,0,
		static_cast<long>(singleWidth * bulletNum),
		static_cast<long>(m_spTex->GetHeight()) };
}

void BulletNumUI::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color, m_pivot);
}
