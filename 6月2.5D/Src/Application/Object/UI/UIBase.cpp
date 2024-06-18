#include "UIBase.h"

UIBase::UIBase()
{
}

UIBase::~UIBase()
{
	Release();
}

void UIBase::Init()
{
	m_pos = {};

	m_mScale = Math::Matrix::Identity;
	m_mTrans = Math::Matrix::Identity;

	m_color = kWhiteColor;
}

void UIBase::Update()
{
}
