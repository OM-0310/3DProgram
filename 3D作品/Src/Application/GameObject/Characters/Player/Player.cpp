#include "Player.h"
#include "../../../GameObject/Camera/TPSCamera/TPSCamera.h"

void Player::Init()
{
	m_spModel	= std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/Characters/Player/Swat.gltf");

	m_pos		= { 0.f,0.f,-1.2f };
	m_moveDir	= {};
	m_moveSpeed = 0.25f;

	// ↓画面中央座標
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	m_walkFlg	= false;
	m_dashFlg	= false;
	m_keyFlg	= false;

	CharaBase::Init();
}

void Player::Update()
{
	// 移動処理
	MoveProcess();

	// 視点切り替え処理
	ChanegeViewPointProcess();

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);
	UpdateRotateByMouse();

	m_moveDir = m_moveDir.TransformNormal(m_moveDir, GetRotationYMatrix());

	m_moveDir.Normalize();

	m_pos		+= m_moveDir * m_moveSpeed;

	m_pos.y -= m_gravity;
	m_gravity += 0.04f;

	m_color = { 1.f,1.f,1.f,m_alpha };

	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mRot		= GetRotationYMatrix() * Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	m_mWorld = m_mRot * m_mTrans;
}

void Player::PostUpdate()
{
	CharaBase::PostUpdate();
}

//================================================================================================================================
// 移動処理・・・ここから
//================================================================================================================================
void Player::MoveProcess()
{
	m_moveDir = Math::Vector3::Zero;

	// 移動処理
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_walkFlg = true;
		m_moveDir += { 0.f, 0.f, 1.f};
	}
	else
	{
		if (!m_walkFlg)
		{
			if (m_dashFlg)m_dashFlg = false;
		}
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_walkFlg = true;
		m_moveDir += { 0.f, 0.f, -1.f};
	}
	else
	{
		if (!m_walkFlg)
		{
			if (m_dashFlg)m_dashFlg = false;
		}
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_walkFlg = true;
		m_moveDir += {-1.f, 0.f, 0.f};
	}
	else
	{
		if (!m_walkFlg)
		{
			if (m_dashFlg)m_dashFlg = false;
		}
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_walkFlg = true;
		m_moveDir += { 1.f, 0.f, 0.f};
	}
	else
	{
		if (!m_walkFlg)
		{
			if (m_dashFlg)	m_dashFlg = false;
		}
	}
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_dashFlg = true;
	}

	if (m_moveDir == Math::Vector3::Zero)	m_walkFlg = false;

	if (m_dashFlg)							m_moveSpeed = 0.5f;
	else									m_moveSpeed = 0.25f;
}
//================================================================================================================================
// 移動処理・・・ここまで
//================================================================================================================================

//================================================================================================================================
// 視点切り替え処理・・・ここから
//================================================================================================================================
void Player::ChanegeViewPointProcess()
{
	const std::shared_ptr<TPSCamera> _spCamere = m_wpCamera.lock();

	// 視点切り替え
	if (GetAsyncKeyState('F') & 0x8000)
	{
		switch (_spCamere->GetCameraType())
		{
		case TPSCamera::CameraType::FPS:
			if (!m_keyFlg)
			{
				m_keyFlg = true;
				m_alpha = 1.0f;
				_spCamere->ChangeTPS();
			}
			break;
		case TPSCamera::CameraType::TPS:
			if (!m_keyFlg)
			{
				m_keyFlg = true;
				m_alpha = 0.0f;
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
}
//================================================================================================================================
// 視点切り替え処理・・・ここまで
//================================================================================================================================

void Player::UpdateRotateByMouse()
{
	// マウスでカメラを回転させる処理
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	POINT _mouseMove{};
	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// 実際にカメラを回転させる処理(0.15はただの補正値)
	m_degAng.x += _mouseMove.y * 0.15f;
	m_degAng.y += _mouseMove.x * 0.15f;

	// 回転制御
	m_degAng.x = std::clamp(m_degAng.x, -45.f, 45.f);
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
