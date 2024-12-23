﻿#include "TPSCamera.h"

#include "../../Characters/Player/Player.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	m_basePoint		= m_tpsRBasePoint;

	m_camType		= CameraType::TpsR;
	m_pastCamType	= m_camType;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);
}

void TPSCamera::Update()
{
	switch (m_camType)
	{
	//case TPSCamera::CameraType::Fps:
	//	m_basePoint = m_fpsBasePoint;
	//	break;
	case TPSCamera::CameraType::TpsR:
		m_basePoint = m_tpsRBasePoint;
		break;
	case TPSCamera::CameraType::TpsL:
		m_basePoint = m_tpsLBasePoint;
		break;
	case TPSCamera::CameraType::AimR:
		m_basePoint = m_aimRBasePoint;
		break;
	case TPSCamera::CameraType::AimL:
		m_basePoint = m_aimLBasePoint;
		break;
	case TPSCamera::CameraType::SitR:
		m_basePoint = m_sitRBasePoint;
		break;
	case TPSCamera::CameraType::SitL:
		m_basePoint = m_sitLBasePoint;
		break;
	case TPSCamera::CameraType::SitAimR:
		m_basePoint = m_sitAimRPoint;
		break;
	case TPSCamera::CameraType::SitAimL:
		m_basePoint = m_sitAimLPoint;
		break;
	default:
		break;
	}

	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								targetMat = Math::Matrix::Identity;
	const std::shared_ptr<const Player>	spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		targetMat = Math::Matrix::CreateTranslation(spPlayer->GetPos());
	}

	// カメラの回転
	UpdateRotateByMouse();
	m_mRot		= GetRotationMatrix();
	m_mLocalPos = Math::Matrix::CreateTranslation(m_basePoint);
	m_mWorld	= (m_mLocalPos * m_mRot) * targetMat;

	CameraBase::Update();
}

//void TPSCamera::ChangeFPS()
//{
//	m_camType		= CameraType::Fps;
//}

void TPSCamera::ChangeTPSR()
{
	m_camType		= CameraType::TpsR;
	m_pastCamType	= CameraType::TpsR;
}

void TPSCamera::ChangeTPSL()
{
	m_camType		= CameraType::TpsL;
	m_pastCamType	= CameraType::TpsL;
}

void TPSCamera::ChangeAimR()
{
	m_camType		= CameraType::AimR;
	m_pastCamType	= CameraType::TpsR;
}

void TPSCamera::ChangeAimL()
{
	m_camType		= CameraType::AimL;
	m_pastCamType	= CameraType::TpsL;
}

void TPSCamera::ChangeSitR()
{
	m_camType		= CameraType::SitR;
	m_pastCamType	= CameraType::SitR;
}

void TPSCamera::ChangeSitL()
{
	m_camType		= CameraType::SitL;
	m_pastCamType	= CameraType::SitL;
}

void TPSCamera::ChangeSitAimR()
{
	m_camType		= CameraType::SitAimR;
	m_pastCamType	= CameraType::SitR;
}

void TPSCamera::ChangeSitAimL()
{
	m_camType		= CameraType::SitAimL;
	m_pastCamType	= CameraType::SitL;
}
