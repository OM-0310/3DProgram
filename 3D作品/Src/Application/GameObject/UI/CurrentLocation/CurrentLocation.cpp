#include "CurrentLocation.h"

#include "../../Characters/Player/Player.h"

void CurrentLocation::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/MiniMap/CurrentLocation/CurrentLocation.png");

	m_rect = { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos = {};
	m_color = { 0.0f,0.0f,1.0f,1.0f };

	m_active = false;
}

void CurrentLocation::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	Math::Vector3 parentPos;

	// プレイヤーの情報があるとき
	if (spPlayer)
	{
		// プレイヤーの座標を取得
		parentPos = spPlayer->GetPos();
	}

	// 座標を調整
	float x = parentPos.x * m_posMulti;
	float y = (parentPos.z * m_posMulti) + m_adjustZPos;

	// 座標を確定
	m_pos = { x,y };

	// 色情報を確定
	m_color = { 0.0f,0.0f,1.0f,1.0f };
}

void CurrentLocation::DrawSprite()
{
	if (!m_active)return;
	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
}
