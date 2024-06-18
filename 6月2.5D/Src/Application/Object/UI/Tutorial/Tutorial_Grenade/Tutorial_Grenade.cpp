#include "Tutorial_Grenade.h"

void Tutorial_Grenade::Init()
{
	m_pos = {};

	m_mScale = Math::Matrix::Identity;
	m_mTrans = Math::Matrix::Identity;

	m_color = kWhiteColor;
}

void Tutorial_Grenade::Update()
{
}
