#include "CardKeyLocation.h"

#include "../../Item/CardKey/CardKey.h"
	
void CardKeyLocation::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/MiniMap/CurrentLocation/CurrentLocation.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= {};
	m_color			= { 0.0f,1.0f,0.0f,1.0f };

	m_active		= false;
	m_permissionFlg = false;
}

void CardKeyLocation::Update()
{
	const std::shared_ptr<CardKey> spCardKey = m_wpCardKey.lock();
	Math::Vector3 parentPos;

	// カードキーの情報があるとき
	if (spCardKey)
	{
		// カードキーの座標を取得
		parentPos = spCardKey->GetPos();
	}

	// 座標を調整
	float x = parentPos.x * m_posMulti;
	float y = (parentPos.z * m_posMulti) + m_adjustZPos;

	// 座標を確定
	m_pos = { x,y };

	// 色を代入
	m_color = { 0.0f,1.0f,0.0f,1.0f };
}

void CardKeyLocation::DrawSprite()
{
	// 表示許可フラグがtrueのとき
	if (m_permissionFlg)
	{
		// フラグがfalseのとき早期リターン
		if (!m_active)return;
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
	}
}
