#include "Player.h"
#include "../../Camera/TPSCamera/TPSCamera.h"
#include "../../Weapon/WeaponBase/WeaponBase.h"
#include "../../../Scene/SceneManager.h"

void Player::Init()
{
	m_spModel		= std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/Models/Characters/Player/Swat.gltf");

	m_pos			= { 0.f,0.f,0.f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.f;

	m_moveFlg		= false;
	m_shotFlg		= false;
	m_keyFlg		= false;
	m_changeKeyFlg	= false;
	m_sitKeyFlg		= false;
	m_creepKeyFlg	= false;

	m_idleFlg		= false;
	m_walkFlg		= false;
	m_runFlg		= false;
	m_readyFlg		= false;
	m_sitFlg		= false;
	m_sitIdleFlg	= false;
	m_sitWalkFlg	= false;
	m_sitReadyFlg	= false;

	m_sitCnt		= 0;

	m_spAnimator	= std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Idle"));

	m_pType			= PlayerType::Idle;
	m_posType		= PostureType::Stand;

	CharaBase::Init();
}

void Player::Update()
{
	const std::shared_ptr<TPSCamera> _spCamere = m_wpCamera.lock();

	Math::Matrix _camRotMat;
	if (_spCamere)
	{
		_camRotMat = _spCamere->GetRotationMatrix();
	}

	// アニメーション切り替え処理
	ChanegeAnimation();

	// 移動処理
	MoveProcess();

	// 体勢切り替え処理
	ChanegePostureProcess();

	// 視点切り替え処理
	ChanegeViewPointProcess();

	// 銃構え処理
	AimProcess();

	// 武器切り替え処理
	ChanegeWeaponProcess();

	// 移動方向が0になったらフラグをfalseにする
	if (m_moveDir == Math::Vector3::Zero)
	{
		m_moveFlg = false;
	}
	if (!m_moveFlg)
	{
		if (!m_shotFlg)
		{
			switch (m_posType)
			{
			case Player::PostureType::Stand:
				m_pType = PlayerType::Idle;
				break;
			case Player::PostureType::Sit:
				if (m_pType != PlayerType::Sit)
				{
					m_pType = PlayerType::Sit_Idle;
				}
				break;
			case Player::PostureType::Creep:
				break;
			}
		}
	}

	if (m_pType == PlayerType::Sit)
	{
		m_sitCnt++;
	}
	else
	{
		m_sitCnt = 0;
	}

	if (m_sitCnt >= 6)
	{
		m_pType = PlayerType::Sit_Idle;
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

	if (!m_shotFlg)
	{
		m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	}
	else
	{
		m_mRot = _camRotMat;
	}
	//m_mAdjustRotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));
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
// アニメーション切り替え処理・・・ここから
//================================================================================================================================
void Player::ChanegeAnimation()
{
	switch (m_pType)
	{
	case PlayerType::Idle:
		if (!m_idleFlg)
		{
			PlayerTypeFlagSwitch(true, false, false, false, false, false, false, false, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Idle"));
		}
		break;
	case PlayerType::Walk:
		if (!m_walkFlg)
		{
			PlayerTypeFlagSwitch(false, true, false, false, false, false, false, false, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Walk"));
		}
		break;
	case PlayerType::Run:
		if (!m_runFlg)
		{
			PlayerTypeFlagSwitch(false, false, true, false, false, false, false, false, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Run"));
		}
		break;
	case PlayerType::Ready:
		if (!m_readyFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, true, false, false, false, false, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Ready"));
		}
		break;
	case PlayerType::Stand:
		if (!m_standFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, true, false, false, false, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Stand"));
		}
		break;
	case PlayerType::Sit:
		if (!m_sitFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, true, false, false, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Sit"));
		}
		break;
	case PlayerType::Sit_Idle:
		if (!m_sitIdleFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, false, true, false, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Sit_Idle"));
		}
		break;
	case PlayerType::Sit_Walk:
		if (!m_sitWalkFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, false, false, true, false, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Sit_Walk"));
		}
		break;
	case PlayerType::Sit_Ready:
		if (!m_sitReadyFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, false, false, false, true, false, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Sit_Ready"));
		}
		break;
	case PlayerType::Creep:
		if (!m_creepFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, false, false, false, false, true, false, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Creep"));
		}
		break;
	case PlayerType::Creep_Idle:
		if (!m_creepIdleFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, false, false, false, false, false, true, false, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Creep_Idle"));
		}
		break;
	case PlayerType::Creep_Walk:
		if (!m_creepWalkFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, false, false, false, false, false, false, true, false);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Creep_Walk"));
		}
		break;
	case PlayerType::Creep_Ready:
		if (!m_creepReadyFlg)
		{
			PlayerTypeFlagSwitch(false, false, false, false, false, false, false, false, false, false, false, false, true);

			m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Creep_Ready"));
		}
		break;
	}
}
//================================================================================================================================
// アニメーション切り替え処理・・・ここまで
//================================================================================================================================

//================================================================================================================================
// 移動処理・・・ここから
//================================================================================================================================
void Player::MoveProcess()
{
	m_moveDir = Math::Vector3::Zero;

	// 移動処理
	if (GetAsyncKeyState('W') & 0x8000)
	{
		m_moveFlg = true;
		m_moveDir += { 0.f, 0.f, 1.f};

		switch (m_posType)
		{
		case Player::PostureType::Stand:

			m_pType		= PlayerType::Run;
			m_moveSpeed = 0.2f;
			if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
			{
				m_pType		= PlayerType::Walk;
				m_moveSpeed = 0.05f;
			}

			break;
		case Player::PostureType::Sit:

			m_pType		= PlayerType::Sit_Walk;
			m_moveSpeed = 0.1f;

			break;
		case Player::PostureType::Creep:
			break;
		}
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		m_moveFlg = true;
		m_moveDir += { 0.f, 0.f, -1.f};

		switch (m_posType)
		{
		case Player::PostureType::Stand:

			m_pType = PlayerType::Run;
			m_moveSpeed = 0.2f;
			if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
			{
				m_pType = PlayerType::Walk;
				m_moveSpeed = 0.05f;
			}

			break;
		case Player::PostureType::Sit:

			m_pType = PlayerType::Sit_Walk;
			m_moveSpeed = 0.1f;

			break;
		case Player::PostureType::Creep:
			break;
		}
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_moveFlg = true;
		m_moveDir += {-1.f, 0.f, 0.f};

		switch (m_posType)
		{
		case Player::PostureType::Stand:

			m_pType = PlayerType::Run;
			m_moveSpeed = 0.2f;
			if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
			{
				m_pType = PlayerType::Walk;
				m_moveSpeed = 0.05f;
			}

			break;
		case Player::PostureType::Sit:

			m_pType = PlayerType::Sit_Walk;
			m_moveSpeed = 0.1f;

			break;
		case Player::PostureType::Creep:
			break;
		}
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		m_moveFlg = true;
		m_moveDir += { 1.f, 0.f, 0.f};

		switch (m_posType)
		{
		case Player::PostureType::Stand:

			m_pType = PlayerType::Run;
			m_moveSpeed = 0.2f;
			if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
			{
				m_pType = PlayerType::Walk;
				m_moveSpeed = 0.05f;
			}

			break;
		case Player::PostureType::Sit:

			m_pType = PlayerType::Sit_Walk;
			m_moveSpeed = 0.1f;

			break;
		case Player::PostureType::Creep:
			break;
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
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	if (GetAsyncKeyState('V') & 0x8000)
	{
		switch (_spCamera->GetCamType())
		{
		case TPSCamera::CameraType::TpsR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamera->ChangeTPSL();
			}
			break;
		case TPSCamera::CameraType::TpsL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamera->ChangeTPSR();
			}
			break;
		case TPSCamera::CameraType::AimR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamera->ChangeAimL();
			}
			break;
		case TPSCamera::CameraType::AimL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamera->ChangeAimR();
			}
			break;
		case TPSCamera::CameraType::SitR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamera->ChangeSitL();
			}
			break;
		case TPSCamera::CameraType::SitL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				_spCamera->ChangeSitR();
			}
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
// 体勢切り替え処理・・・ここから
//================================================================================================================================
void Player::ChanegePostureProcess()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	if (GetAsyncKeyState('C') & 0x8000)
	{
		// キーフラグがfalseだったらtrueにする
		if (!m_sitKeyFlg)
		{
			m_sitKeyFlg = true;
			switch (m_posType)
			{
			case Player::PostureType::Stand:
				m_posType	= PostureType::Sit;
				m_pType		= PlayerType::Sit;
				switch (_spCamera->GetCamType())
				{
				case TPSCamera::CameraType::TpsR:
					_spCamera->ChangeSitR();
					break;
				case TPSCamera::CameraType::TpsL:
					_spCamera->ChangeSitL();
					break;
				case TPSCamera::CameraType::AimR:
					_spCamera->ChangeSitAimR();
					break;
				case TPSCamera::CameraType::AimL:
					_spCamera->ChangeSitAimL();
					break;
				}
				break;
			case Player::PostureType::Sit:
				m_posType = PostureType::Stand;
				// blender側で実装
				//m_pType = PlayerType::Stand;
				switch (_spCamera->GetCamType())
				{
				case TPSCamera::CameraType::SitR:
					_spCamera->ChangeTPSR();
					break;
				case TPSCamera::CameraType::SitL:
					_spCamera->ChangeTPSL();
					break;
				case TPSCamera::CameraType::SitAimR:
					_spCamera->ChangeAimR();
					break;
				case TPSCamera::CameraType::SitAimL:
					_spCamera->ChangeAimL();
					break;
				}
				break;
			case Player::PostureType::Creep:
				m_posType = PostureType::Sit;
				// blender側で実装
				//m_pType = PlayerType::Creep;
				
				//switch (_spCamera->GetCamType())
				//{
				//case TPSCamera::CameraType::CreepR:
				//	_spCamera->ChangeSitR();
				//	break;
				//case TPSCamera::CameraType::CreepL:
				//	_spCamera->ChangeSitL();
				//	break;
				//case TPSCamera::CameraType::CreepAimR:
				//	_spCamera->ChangeSitAimR();
				//	break;
				//case TPSCamera::CameraType::CreepAimL:
				//	_spCamera->ChangeSitAimL();
				//	break;
				//default:
				//	break;
				//}
				break;
			}
		}
	}
	else
	{
		m_sitKeyFlg = false;
	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		// キーフラグがfalseだったらtrueにする
		if (!m_creepKeyFlg)
		{
			m_creepKeyFlg = true;
			switch (m_posType)
			{
			case Player::PostureType::Stand:
				m_posType = PostureType::Creep;
				// blender側で実装
				//m_pType = PlayerType::Creep;
				switch (_spCamera->GetCamType())
				{
				case TPSCamera::CameraType::TpsR:
					break;
				case TPSCamera::CameraType::TpsL:
					break;
				case TPSCamera::CameraType::AimR:
					break;
				case TPSCamera::CameraType::AimL:
					break;
				}
				break;
			case Player::PostureType::Sit:
				m_posType = PostureType::Creep;
				// blender側で実装
				//m_pType = PlayerType::Creep;
				switch (_spCamera->GetCamType())
				{
				case TPSCamera::CameraType::SitR:
					break;
				case TPSCamera::CameraType::SitL:
					break;
				case TPSCamera::CameraType::SitAimR:
					break;
				case TPSCamera::CameraType::SitAimL:
					break;
				}
				break;
			case Player::PostureType::Creep:
				m_posType = PostureType::Stand;
				// blender側で実装
				//m_pType = PlayerType::Stand;
				switch (_spCamera->GetCamType())
				{
				case TPSCamera::CameraType::CreepR:
					break;
				case TPSCamera::CameraType::CreepL:
					break;
				case TPSCamera::CameraType::CreepAimR:
					break;
				case TPSCamera::CameraType::CreepAimL:
					break;
				}
				break;
			}
		}
	}
	else
	{
		m_creepKeyFlg = false;
	}

	// プレイヤーの状態がSitの場合にカウントをプラスする
	//if (m_pType == PlayerType::Sit)
	//{
	//	m_sitCnt++;
	//}
	//if (m_sitCnt >= 6)
	//{
	//	m_pType = PlayerType::Sit_Idle;
	//}
}
//================================================================================================================================
// 体勢切り替え処理・・・ここまで
//================================================================================================================================

//================================================================================================================================
// 銃構え処理・・・ここから
//================================================================================================================================
void Player::AimProcess()
{
	//std::shared_ptr<WeaponBase> _spWeapon = m_wpWeapon.lock();
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_shotFlg	= true;
		switch (m_posType)
		{
		case Player::PostureType::Stand:
			m_pType = PlayerType::Ready;
			if (_spCamera->GetCamType() == TPSCamera::CameraType::TpsR)
			{
				_spCamera->ChangeAimR();
			}
			else if (_spCamera->GetCamType() == TPSCamera::CameraType::TpsL)
			{
				_spCamera->ChangeAimL();
			}
			break;
		case Player::PostureType::Sit:
			m_pType = PlayerType::Sit_Ready;
			if (_spCamera->GetCamType() == TPSCamera::CameraType::SitR)
			{
				_spCamera->ChangeSitAimR();
			}
			else if (_spCamera->GetCamType() == TPSCamera::CameraType::SitL)
			{
				_spCamera->ChangeSitAimL();
			}
			break;
		case Player::PostureType::Creep:
			break;
		default:
			break;
		}
		//_spWeapon->Hold();
	}
	else
	{
		m_shotFlg	= false;

		switch (_spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::TpsR:
			_spCamera->ChangeTPSR();
			break;
		case TPSCamera::CameraType::TpsL:
			_spCamera->ChangeTPSL();
			break;
		case TPSCamera::CameraType::SitR:
			_spCamera->ChangeSitR();
			break;
		case TPSCamera::CameraType::SitL:
			_spCamera->ChangeSitL();
			break;
		default:
			break;
		}
		//_spWeapon->UnHold();
	}
}
//================================================================================================================================
// 銃構え処理・・・ここまで
//================================================================================================================================


//================================================================================================================================
// 弾発射処理・・・ここから
//================================================================================================================================
void Player::ShotProcess()
{
	if (m_pType == PlayerType::Ready)
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{

		}
	}
}
//================================================================================================================================
// 弾発射処理・・・ここまで
//================================================================================================================================

void Player::ChanegeWeaponProcess()
{
}

//================================================================================================================================
// プレイヤー状態フラグ切り替え・・・ここから
//================================================================================================================================
void Player::PlayerTypeFlagSwitch(	
									bool _idleFlg,		bool _walkFlg,	bool _runFlg,		bool _readyFlg,
									bool _standFlg,		bool _sitFlg,	bool _sitIdleFlg,	bool _sitWalkFlg,	
									bool _sitReadyFlg,	bool _creepFlg, bool _creepIdleFlg,	bool _creepWalkFlg,	bool _creepReadyFlg
								 )
{
	m_idleFlg		= _idleFlg;
	m_walkFlg		= _walkFlg;
	m_runFlg		= _runFlg;
	m_readyFlg		= _readyFlg;
	m_standFlg		= _standFlg;
	m_sitFlg		= _sitFlg;
	m_sitIdleFlg	= _sitIdleFlg;
	m_sitWalkFlg	= _sitWalkFlg;
	m_sitReadyFlg	= _sitReadyFlg;
	m_creepFlg		= _creepFlg;
	m_creepIdleFlg	= _creepIdleFlg;
	m_creepWalkFlg	= _creepWalkFlg;
	m_creepReadyFlg = _creepReadyFlg;
}
//================================================================================================================================
// プレイヤー状態フラグ切り替え・・・ここまで
//================================================================================================================================
