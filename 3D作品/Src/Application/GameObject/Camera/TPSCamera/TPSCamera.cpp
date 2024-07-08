#include "TPSCamera.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	m_basePoint = m_tpsBasePoint;

	// 基準点(ターゲット)の目線
	m_mLocalPos = Math::Matrix::CreateTranslation(m_basePoint);

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	m_fpsFlg = false;
	m_tpsFlg = false;

	m_camType = CameraType::TPS;
}

void TPSCamera::Update()
{
	switch (m_camType)
	{
	case TPSCamera::CameraType::FPS:
		m_basePoint = m_fpsBasePoint;
		break;
	case TPSCamera::CameraType::TPS:
		m_basePoint = m_tpsBasePoint;
		break;
	default:
		break;
	}

	// ターゲットの行列(有効な場合利用する)
	Math::Matrix								_targetMat = Math::Matrix::Identity;
	const std::shared_ptr<const KdGameObject>	_spTarget = m_wpTarget.lock();
	if (_spTarget)
	{
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	// カメラの回転
	UpdateRotateByMouse();
	m_mRotation = GetRotationMatrix();
	m_mLocalPos = Math::Matrix::CreateTranslation(m_basePoint);
	m_mWorld = (m_mLocalPos * m_mRotation) *  _targetMat;

	CameraBase::Update();
}