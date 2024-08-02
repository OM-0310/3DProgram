#include "Player.h"
#include "../../Camera/TPSCamera/TPSCamera.h"
#include "../../Weapon/WeaponBase/WeaponBase.h"
#include "../../../Scene/SceneManager.h"

void Player::Init()
{
	m_spModel		= std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Characters/Player/Swat.gltf");

	m_pos			= { 0.f,0.f,-1.2f };
	m_moveDir		= {};
	m_moveSpeed		= 0.2f;

	m_moveFlg		= false;
	m_dashFlg		= false;
	m_keyFlg		= false;
	m_changeKeyFlg	= false;

	m_idleFlg		= false;
	m_runFlg		= false;

	m_spAnimator = std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Idle"));

	m_pType = PlayerType::Idle;

	CharaBase::Init();
}

void Player::Update()
{
	const std::shared_ptr<TPSCamera> _spCamere = m_wpCamera.lock();

	switch (m_pType)
	{
	case PlayerType::Idle:
		if (!m_idleFlg)
		{
			m_idleFlg	= true;
			m_walkFlg	= false;
			m_runFlg	= false;
			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Idle"));
		}
		break;
	case PlayerType::Walk:
		if (!m_walkFlg)
		{
			m_walkFlg	= true;
			m_idleFlg	= false;
			m_runFlg	= false;
			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Walk"));
		}
		break;
	case PlayerType::Run:
		if (!m_runFlg)
		{
			m_runFlg	= true;
			m_idleFlg	= false;
			m_walkFlg	= false;
			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Run"));
		}
		break;
	default:
		break;
	}

	// 移動処理
	MoveProcess();

	// 視点切り替え処理
	ChanegeViewPointProcess();

	// 銃構え処理
	AimProcess();

	// 武器切り替え処理
	ChanegeWeaponProcess();

	if (m_moveDir == Math::Vector3::Zero)
	{
		m_pType = PlayerType::Idle;
		m_moveFlg = false;
	}

	if (_spCamere)
	{
		m_moveDir = m_moveDir.TransformNormal(m_moveDir, _spCamere->GetRotationYMatrix());
	}

	m_moveDir.Normalize();

	m_pos		+= m_moveDir * m_moveSpeed;

	m_pos.y		-= m_gravity;
	m_gravity	+= 0.04f;

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);

	if (_spCamere->GetCamType() == TPSCamera::CameraType::TpsL ||
		_spCamere->GetCamType() == TPSCamera::CameraType::TpsR)
	{
		m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	}
	else if (_spCamere->GetCamType() == TPSCamera::CameraType::AimL ||
			_spCamere->GetCamType() == TPSCamera::CameraType::AimR)
	{
		//m_mRot = GetRotationYMatrix();
	}
	m_mAdjustRotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
	m_mTrans		= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld		= m_mRot * m_mTrans;
}

void Player::PostUpdate()
{
	// アニメーションの更新
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();

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
		m_pType = PlayerType::Run;
		m_moveFlg = true;
		m_moveDir += { 0.f, 0.f, 1.f};

		if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		{
			m_pType = PlayerType::Walk;
			m_moveSpeed = 0.05f;
		}
		else
		{
			m_moveSpeed = 0.2f;
		}
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_pType = PlayerType::Run;
		m_moveFlg = true;
		m_moveDir += { 0.f, 0.f, -1.f};

		if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		{
			m_pType = PlayerType::Walk;
			m_moveSpeed = 0.05f;
		}
		else
		{
			m_moveSpeed = 0.2f;
		}
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_pType = PlayerType::Run;
		m_moveFlg = true;
		m_moveDir += {-1.f, 0.f, 0.f};

		if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		{
			m_pType = PlayerType::Walk;
			m_moveSpeed = 0.05f;
		}
		else
		{
			m_moveSpeed = 0.2f;
		}
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_pType = PlayerType::Run;
		m_moveFlg = true;
		m_moveDir += { 1.f, 0.f, 0.f};

		if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		{
			m_pType = PlayerType::Walk;
			m_moveSpeed = 0.05f;
		}
		else
		{
			m_moveSpeed = 0.2f;
		}
	}
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
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		//if (GetAsyncKeyState('F') & 0x8000)
		//{
		//	switch (_spCamere->GetCamType())	// カメラのタイプによって切り替える
		//	{
		//	case TPSCamera::CameraType::Fps:
		//		if (!m_keyFlg)
		//		{
		//			m_keyFlg = true;
		//			m_alpha = 1.0f;
		//			if (_spCamere->GetPastCamType() == TPSCamera::CameraType::TpsL)
		//			{
		//				_spCamere->ChangeAimL();
		//			}
		//			else if (_spCamere->GetPastCamType() == TPSCamera::CameraType::TpsR)
		//			{
		//				_spCamere->ChangeAimR();
		//			}
		//		}
		//		break;
		//	case TPSCamera::CameraType::TpsL:
		//		if (!m_keyFlg)
		//		{
		//			m_keyFlg = true;
		//			m_alpha = 0.0f;
		//			_spCamere->ChangeFPS();
		//		}
		//		break;
		//	case TPSCamera::CameraType::TpsR:
		//		if (!m_keyFlg)
		//		{
		//			m_keyFlg = true;
		//			m_alpha = 0.0f;
		//			_spCamere->ChangeFPS();
		//		}
		//		break;
		//	case TPSCamera::CameraType::AimL:
		//		if (!m_keyFlg)
		//		{
		//			m_keyFlg = true;
		//			m_alpha = 0.0f;
		//			_spCamere->ChangeFPS();
		//		}
		//		break;
		//	case TPSCamera::CameraType::AimR:
		//		if (!m_keyFlg)
		//		{
		//			m_keyFlg = true;
		//			m_alpha = 0.0f;
		//			_spCamere->ChangeFPS();
		//		}
		//		break;
		//	default:
		//		break;
		//	}
		//}
		//else
		//{
		//	m_keyFlg = false;
		//}
	}
	else
	{
		m_alpha = 1.f;
		if (_spCamere->GetPastCamType() == TPSCamera::CameraType::TpsL)
		{
			_spCamere->ChangeTPSL();
		}
		else if (_spCamere->GetPastCamType() == TPSCamera::CameraType::TpsR)
		{
			_spCamere->ChangeTPSR();
		}
	}

	if (GetAsyncKeyState('V') & 0x8000)
	{
		switch (_spCamere->GetCamType())
		{
		case TPSCamera::CameraType::TpsL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamere->ChangeTPSR();
			}
			break;
		case TPSCamera::CameraType::TpsR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamere->ChangeTPSL();
			}
			break;
		case TPSCamera::CameraType::AimL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamere->ChangeAimR();
			}
			break;
		case TPSCamera::CameraType::AimR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamere->ChangeAimL();
			}
			break;
		default:
			break;
		}
	}
	else
	{
		m_changeKeyFlg = false;
	}
}
//================================================================================================================================
// 視点切り替え処理・・・ここまで
//================================================================================================================================

//================================================================================================================================
// 銃構え処理・・・ここから
//================================================================================================================================
void Player::AimProcess()
{
	std::shared_ptr<WeaponBase> _spWeapon = m_wpWeapon.lock();
	std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		if (_spCamera->GetCamType() == TPSCamera::CameraType::TpsR)
		{
			_spCamera->ChangeAimR();
		}
		else if (_spCamera->GetCamType() == TPSCamera::CameraType::TpsL)
		{
			_spCamera->ChangeAimL();
		}
		_spWeapon->Hold();
		
	}
	else
	{
		_spWeapon->UnHold();
	}
}
//================================================================================================================================
// 銃構え処理・・・ここまで
//================================================================================================================================

void Player::ChanegeWeaponProcess()
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
