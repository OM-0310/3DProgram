#include "TitleCamera.h"

void TitleCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	m_mRot = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(10));

	// 基準点(ターゲット)の目線
	m_mLocalPos = Math::Matrix::CreateTranslation(0.f, 1.0f, -10.0f);
}

void TitleCamera::Update()
{
	m_mWorld	= m_mRot * m_mLocalPos;

	CameraBase::Update();
}
