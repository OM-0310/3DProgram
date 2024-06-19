#include "TrackingCamera.h"

void TrackingCamera::Init()
{
	// 親クラスの初期化
	CameraBase::Init();

	// 基準点(ターゲット)からどれだけ離れているか
	m_mLocalPos = Math::Matrix::CreateTranslation(0.0f, 6.0f, -5.0f);

	// どれだけ傾けているか
	m_mRotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(45));
}

void TrackingCamera::Update()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix _targetMat = Math::Matrix::Identity;
	const std::shared_ptr<KdGameObject> _spTarget = m_wpTarget.lock();
	if (_spTarget)
	{
		//_targetMat = _spTarget->GetMatrix();
		_targetMat = Math::Matrix::CreateTranslation(_spTarget->GetPos());
	}

	m_mWorld = m_mRotation * m_mLocalPos * _targetMat;

	//	親クラスの更新呼び出し
	CameraBase::Update();
}
