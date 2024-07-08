#include "Player.h"
#include "../../../GameObject/Camera/TPSCamera/TPSCamera.h"

void Player::Init()
{
	m_spModel	= std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/Characters/Player/Swat.gltf");

	m_pos		= {0.f,0.f,-1.2f};
	m_moveDir	= {};
	m_moveSpeed = -0.5f;

	m_keyFlg	= false;

	CharaBase::Init();
}

void Player::Update()
{
	const std::shared_ptr<TPSCamera> _spCamere = m_wpCamera.lock();

	m_moveDir = Math::Vector3::Zero;

	if (GetAsyncKeyState('W') & 0x8000) { m_moveDir += { 0.f, 0.f, -1.f}; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveDir += { 0.f, 0.f, 1.f}; }
	if (GetAsyncKeyState('A') & 0x8000) { m_moveDir += { 1.f, 0.f, 0.f}; }
	if (GetAsyncKeyState('D') & 0x8000) { m_moveDir += {-1.f, 0.f, 0.f}; }

	// 視点切り替え
	if (GetAsyncKeyState('F') & 0x8000)
	{
		switch (_spCamere->GetCameraType())
		{
		case TPSCamera::CameraType::FPS:
			if (!m_keyFlg)
			{
				m_keyFlg = true;
				_spCamere->ChangeTPS();
			}
			break;
		case TPSCamera::CameraType::TPS:
			if (!m_keyFlg)
			{
				m_keyFlg = true;
				_spCamere->ChangeFPS();
			}
			break;
		default:
			break;
		}
	}
	else
	{
		m_keyFlg = false;
	}

	// ベクトルの向きをY軸の回転行列で変換
	if (_spCamere)
	{
		m_moveDir = m_moveDir.TransformNormal(m_moveDir, _spCamere->GetRotationYMatrix());
	}

	m_moveDir.Normalize();

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);

	m_pos		+= m_moveDir * m_moveSpeed;

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	m_mWorld = m_mRot * m_mTrans;
}

void Player::PostUpdate()
{
}

void Player::UpdateRotate(const Math::Vector3& srcMoveVec)
{
	// 何も入力がない場合は処理しない
	if (srcMoveVec.LengthSquared() == 0) return;

	// キャラの正面方向のベクトル		// 社会に出たときはForward()を使用する
	Math::Vector3 _nowDir = GetMatrix().Backward();
	_nowDir.Normalize();

	// 移動方向のベクトル
	Math::Vector3 _targetDir = srcMoveVec;
	_targetDir.Normalize();

	float _nowAng = atan2(_nowDir.x, _nowDir.z);
	_nowAng = DirectX::XMConvertToDegrees(_nowAng);

	float _targetAng = atan2(_targetDir.x, _targetDir.z);
	_targetAng = DirectX::XMConvertToDegrees(_targetAng);

	// 角度の差分を図る
	float _betweenAng = _targetAng - _nowAng;
	if (_betweenAng > 180)
	{
		_betweenAng -= 360;
	}
	else if (_betweenAng < -180)
	{
		_betweenAng += 360;
	}
	float _rotateAng = std::clamp(_betweenAng, -8.0f, 8.0f);
	m_angle.y += _rotateAng;
}
