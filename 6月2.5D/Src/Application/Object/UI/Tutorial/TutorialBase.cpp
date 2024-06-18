#include "TutorialBase.h"

void TutorialBase::Init()
{
	m_pos = {};

	m_mScale = Math::Matrix::Identity;
	m_mTrans = Math::Matrix::Identity;

	m_color = kWhiteColor;
}

void TutorialBase::Update()
{
}

void TutorialBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
