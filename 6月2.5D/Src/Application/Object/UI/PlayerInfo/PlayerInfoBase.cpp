#include "PlayerInfoBase.h"

void PlayerInfoBase::Init()
{
	m_pos = {};
	m_rectPos = {};

	m_mScale = Math::Matrix::Identity;
	m_mTrans = Math::Matrix::Identity;

	m_rect = {};
	m_color = kWhiteColor;
}

void PlayerInfoBase::Update()
{
}

void PlayerInfoBase::DrawSprite()
{
	KdShaderManager::Instance().m_spriteShader.SetMatrix(m_mWorld);

	KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, m_rectPos.x, m_rectPos.y, &m_rect, 0);
}
