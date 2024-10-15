#include "Reticle.h"

void Reticle::Init()
{
	m_spTex = std::make_shared<KdTexture>();
	m_spTex->Load("Asset/Textures/UI/Reticle/Reticle.png");

	m_pos = Math::Vector2::Zero;

	m_color = kWhiteColor;

	m_Active = false;
}

void Reticle::DrawSprite()
{
	if (!m_Active)return;

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, (int)m_pos.x, (int)m_pos.y);
}
