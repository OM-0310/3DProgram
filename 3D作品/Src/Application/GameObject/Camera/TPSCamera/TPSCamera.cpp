#include "TPSCamera.h"

#include "../../Characters/Player/Player.h"

void TPSCamera::Init()
{
	// 親クラスの初期化呼び出し
	CameraBase::Init();

	m_basePoint = m_tpsBasePoint;

	// 基準点(ターゲット)の目線
	m_mLocalPos = Math::Matrix::CreateTranslation(m_basePoint);

	//SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

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
	const std::shared_ptr<const Player>	_spPlayer = m_wpPlayer.lock();
	if (_spPlayer)
	{
		//_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
		_targetMat = _spPlayer->GetRotationMatrix() * Math::Matrix::CreateTranslation(_spPlayer->GetPos());
	}

	// カメラの回転
	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	m_mLocalPos = Math::Matrix::CreateTranslation(m_basePoint);
	m_mWorld = m_mRot * m_mLocalPos * _targetMat;

	CameraBase::Update();
}