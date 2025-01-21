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

	if (spCardKey)
	{
		parentPos = spCardKey->GetPos();
	}

	float x = parentPos.x * m_posMulti;
	float y = (parentPos.z * m_posMulti) + m_adjustZPos;

	m_pos = { x,y };

	m_color = { 0.0f,1.0f,0.0f,1.0f };
}

void CardKeyLocation::DrawSprite()
{
	if (m_permissionFlg)
	{
		if (!m_active)return;
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
	}
}
