#include "CharaBase.h"

CharaBase::CharaBase()
{
}

CharaBase::~CharaBase()
{
	Release();
}

void CharaBase::Update()
{
}

void CharaBase::PostUpdate()
{
}

void CharaBase::Init()
{
	m_mTrans = Math::Matrix::Identity;
	m_mScale = Math::Matrix::Identity;
	m_color = kWhiteColor;
}

void CharaBase::DrawLit()
{
	if (!m_aliveFlg)return;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld, m_color);
}

void CharaBase::GenerateDepthMapFromLight()
{
	if (!m_aliveFlg)return;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void CharaBase::SetHPDec(int _decNum)
{
}

void CharaBase::Release()
{
	m_spPoly = nullptr;
}
