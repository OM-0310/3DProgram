#include "Player.h"
#include "../../Camera/TPSCamera/TPSCamera.h"
#include "../../Weapon/WeaponBase/WeaponBase.h"
#include "../../UI/Reticle/Reticle.h"
#include "../../Bullet/Bullet.h"

#include "../../../Scene/SceneManager.h"

void Player::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/Characters/Player/Swat.gltf");

		m_spAnimator = std::make_shared<KdAnimator>();
	}

	ChangeActionState(std::make_shared<ActionIdle>());

	m_pos			= { 0.f,0.f,0.f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.f;

	m_moveFlg		= false;
	m_shotFlg		= false;
	m_keyFlg		= false;
	m_changeKeyFlg	= false;
	m_posKeyFlg		= false;
	m_creepKeyFlg	= false;

	m_standCnt		= 0;
	m_sitCnt		= 0;
	m_shotCnt		= 0;

	//m_pType			= PlayerType::Idle;
	m_posType		= PostureType::Stand;

	CharaBase::Init();

	if (m_spModel)
	{
		const KdModelWork::Node* pNode = m_spModel->FindNode("PistolDisarmPoint");

		if (pNode)
		{
			m_mUnHold = pNode->m_worldTransform * m_mTrans;
		}
		else if (!pNode)
		{
			assert(0 && "PistolDisarmPoint 指定したノードが見つかりません");
		}
	}

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Update()
{
	m_gravity	 += 0.04f;
	m_mWorld._42 -= m_gravity;

	// 視点切り替え処理
	ChanegeViewPointProcess();

	// 各種「状態」に応じた更新処理を実行する
	if (m_nowState)
	{
		m_nowState->Update(*this);
	}

	Math::Vector3 disarmPos;
	disarmPos = (m_mUnHold * GetMatrix()).Translation();

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = disarmPos;
	sphereInfo.m_sphere.Radius = 0.05f;
	sphereInfo.m_type = KdCollider::TypeDamage;

	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kBlueColor);

	UpdateCollision();

	//// アニメーション切り替え処理
	//ChanegeAnimation(m_pType);

	//// 移動処理
	//MoveProcess();

	//// 回避処理
	//AvoidProcess();

	//// 体勢切り替え処理
	//ChanegePostureProcess();


	//// 銃構え処理
	//AimProcess();

	//// 武器切り替え処理
	//ChanegeWeaponProcess();

	//// 移動方向が0になったらフラグをfalseにする
	//if (m_moveDir == Math::Vector3::Zero)
	//{
	//	m_moveFlg = false;
	//}
	//if (!m_moveFlg)
	//{
	//	if (!m_shotFlg)
	//	{
	//		switch (m_posType)
	//		{
	//		case Player::PostureType::Stand:
	//			m_pType = PlayerType::Idle;
	//			break;
	//		case Player::PostureType::Sit:
	//			if (m_pType != PlayerType::Sit)
	//			{
	//				m_pType = PlayerType::Sit_Idle;
	//			}
	//			break;
	//		case Player::PostureType::Creep:
	//			break;
	//		}
	//	}
	//}

	//if (m_pType == PlayerType::Sit)
	//{
	//	m_sitCnt++;
	//}
	//else
	//{
	//	m_sitCnt = 0;
	//}

	//if (m_sitCnt >= 6)
	//{
	//	m_pType = PlayerType::Sit_Idle;
	//}

	//if (!m_shotFlg)
	//{
	//}
	//else
	//{
	//}
	//m_mAdjustRotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(180));

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;
}

void Player::PostUpdate()
{
	// アニメーションの更新
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();

	//CharaBase::PostUpdate();
}

void Player::UpdateCollision()
{
	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	rayInfo.m_dir = Math::Vector3::Down;
	rayInfo.m_pos.y -= 0.01f;

	//float enableStepHigh = 0.2f;
	//rayInfo.m_pos.y += enableStepHigh;

	rayInfo.m_range = m_gravity;
	rayInfo.m_type = KdCollider::TypeGround;

	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}

	float maxOverLap = 0;
	Math::Vector3 hitPos;
	bool ishit = false;
	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			ishit = true;
		}
	}
	if (ishit)
	{
		m_pos = hitPos + Math::Vector3(0, 0.01f, 0);
		m_gravity = 0;
	}
}

//================================================================================================================================
// アニメーション切り替え処理・・・ここから
//================================================================================================================================
void Player::ChanegeAnimation(const std::string& _name)
{
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name));
}
//================================================================================================================================
// アニメーション切り替え処理・・・ここまで
//================================================================================================================================

void Player::StateIdleProc()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	m_moveDir = Math::Vector3::Zero;

	// ActionRunに切り替え
	if (GetAsyncKeyState('W') & 0x8000) { m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveDir += Math::Vector3::Forward;  }
	if (GetAsyncKeyState('A') & 0x8000) { m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { m_moveDir += Math::Vector3::Right;	}
	if (m_moveDir.LengthSquared() > 0)	
	{
		ChangeActionState(std::make_shared<ActionRun>()); 
		return;
	}

	// ActionSitに切り替え
	if (GetAsyncKeyState('C') & 0x8000) 
	{
		if (!m_posKeyFlg)
		{
			m_posKeyFlg = true;
			ChangeActionState(std::make_shared<ActionSit>());
			switch (_spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				_spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				_spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		m_posKeyFlg = false;
	}

	// ActionReadyに切り替え
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeActionState(std::make_shared<ActionReady>());
		return;
	}
}

void Player::StateWalkProc()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	Math::Matrix _camRotMat;
	if (_spCamera)
	{
		_camRotMat = _spCamera->GetRotationMatrix();
	}

	m_moveSpeed = 0.05f;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		m_moveDir = Math::Vector3::Zero;

		if (GetAsyncKeyState('W') & 0x8000) { m_moveDir += Math::Vector3::Backward; }
		if (GetAsyncKeyState('S') & 0x8000) { m_moveDir += Math::Vector3::Forward;  }
		if (GetAsyncKeyState('A') & 0x8000) { m_moveDir += Math::Vector3::Left;		}
		if (GetAsyncKeyState('D') & 0x8000) { m_moveDir += Math::Vector3::Right;	}
	}
	else
	{
		if (m_moveDir.LengthSquared() == 0)
		{
			ChangeActionState(std::make_shared<ActionIdle>());
			return;
		}
		else
		{
			ChangeActionState(std::make_shared<ActionRun>());
			return;
		}
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!m_posKeyFlg)
		{
			m_posKeyFlg = true;
			ChangeActionState(std::make_shared<ActionSit>());
			switch (_spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				_spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				_spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		m_posKeyFlg = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeActionState(std::make_shared<ActionReady>());
	}

	if (_spCamera)
	{
		m_moveDir = m_moveDir.TransformNormal(m_moveDir, _spCamera->GetRotationYMatrix());
	}

	m_moveDir.Normalize();
	m_pos += m_moveDir * m_moveSpeed;

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);

	//m_mRot = _camRotMat;
	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
}

void Player::StateRunProc()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	m_moveSpeed = 0.2f;

	m_moveDir = Math::Vector3::Zero;

	if (GetAsyncKeyState('W') & 0x8000) { m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { m_moveDir += Math::Vector3::Right;	}
	if (m_moveDir.LengthSquared() == 0) 
	{
		ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		if (m_posType == PostureType::Stand)
		{
			ChangeActionState(std::make_shared<ActionWalk>());
			return;
		}
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!m_posKeyFlg)
		{
			m_posKeyFlg = true;
			ChangeActionState(std::make_shared<ActionSit>());
			switch (_spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				_spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				_spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		m_posKeyFlg = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeActionState(std::make_shared<ActionReady>());
	}

	if (_spCamera)
	{
		m_moveDir = m_moveDir.TransformNormal(m_moveDir, _spCamera->GetRotationYMatrix());
	}

	m_moveDir.Normalize();
	m_pos += m_moveDir * m_moveSpeed;

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);

	//m_mRot = _camRotMat;
	m_mRot		= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
}

void Player::StateReadyProc()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();
	const std::shared_ptr<Reticle>	_spReticle = m_wpReticle.lock();

	Math::Matrix _camRotMat;
	if (_spCamera)
	{
		_camRotMat = _spCamera->GetRotationMatrix();
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_spReticle->SetActive(true);

		switch (_spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::TpsR:
			_spCamera->ChangeAimR();
			break;
		case TPSCamera::CameraType::TpsL:
			_spCamera->ChangeAimL();
			break;
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!m_shotFlg)
			{
				m_shotFlg = true;
				ChangeActionState(std::make_shared<ActionShot>());
			}
		}
		else
		{
			m_shotFlg = false;
		}
	}
	else
	{
		_spReticle->SetActive(false);

		switch (_spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::TpsR:
			_spCamera->ChangeTPSR();
			break;
		case TPSCamera::CameraType::TpsL:
			_spCamera->ChangeTPSL();
			break;
		}
		//_spWeapon->UnHold();

		ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	m_mRot = _camRotMat;
}

void Player::StateShotProc()
{
	m_shotCnt++;

	KdCollider::RayInfo ray;
	ray.m_pos	= m_pos;
	ray.m_dir	= m_mWorld.Backward();
	ray.m_range = 1000.0f;
	ray.m_type	= KdCollider::TypeDamage;

	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(ray, &retRayList);
	}

	float maxOverLap = 0.0f;
	Math::Vector3 hitPos = {};

	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap	= ret.m_overlapDistance;
			hitPos		= ret.m_hitPos;
		}
	}

	Math::Vector3 bulletDir = hitPos - m_pos;

	if (m_shotCnt == 1)
	{
		ShotBullet(m_pos, bulletDir);
	}

	if (m_shotCnt >= SHOTCNTMAX)
	{
		ChangeActionState(std::make_shared<ActionReady>());
	}
}

void Player::StateStandProc()
{
	m_standCnt++;

	if (m_standCnt >= STANDCNTMAX)
	{
		ChangeActionState(std::make_shared<ActionIdle>());
	}
}

void Player::StateSitProc()
{
	m_sitCnt++;
	
	if (m_sitCnt >= SITCNTMAX)
	{
		ChangeActionState(std::make_shared<ActionSit_Idle>());
	}
}

void Player::StateSitIdleProc()
{
	std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	m_moveDir = Math::Vector3::Zero;

	// ActionSit_Runに切り替え
	if (GetAsyncKeyState('W') & 0x8000) { m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { m_moveDir += Math::Vector3::Right;	}
	if (m_moveDir.LengthSquared() > 0)
	{
		ChangeActionState(std::make_shared<ActionSit_Walk>());
		return;
	}

	// ActionStandに切り替え
	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!m_posKeyFlg)
		{
			m_posKeyFlg = true;
			ChangeActionState(std::make_shared<ActionStand>());
			switch (_spCamera->GetCamType())
			{
			case TPSCamera::CameraType::SitL:
				_spCamera->ChangeTPSL();
				break;
			case TPSCamera::CameraType::SitR:
				_spCamera->ChangeTPSR();
				break;
			}
			return;
		}
	}
	else
	{
		m_posKeyFlg = false;
	}

	// ActionSit_Readyに切り替え
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeActionState(std::make_shared<ActionSit_Ready>());
		return;
	}
}

void Player::StateSitWalkProc()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();

	m_moveSpeed = 0.1f;

	m_moveDir = Math::Vector3::Zero;

	if (GetAsyncKeyState('W') & 0x8000) { m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { m_moveDir += Math::Vector3::Right;	}
	if (m_moveDir.LengthSquared() == 0)
	{
		ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!m_posKeyFlg)
		{
			m_posKeyFlg = true;
			ChangeActionState(std::make_shared<ActionStand>());
			switch (_spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				_spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				_spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		m_posKeyFlg = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		ChangeActionState(std::make_shared<ActionSit_Ready>());
	}

	if (_spCamera)
	{
		m_moveDir = m_moveDir.TransformNormal(m_moveDir, _spCamera->GetRotationYMatrix());
	}

	m_moveDir.Normalize();
	m_pos += m_moveDir * m_moveSpeed;

	//	キャラクターの回転角度を計算する
	UpdateRotate(m_moveDir);

	//m_mRot = _camRotMat;
	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
}

void Player::StateSitReadyProc()
{
	const std::shared_ptr<TPSCamera> _spCamera = m_wpCamera.lock();
	const std::shared_ptr<Reticle>	_spReticle = m_wpReticle.lock();

	Math::Matrix _camRotMat;
	if (_spCamera)
	{
		_camRotMat = _spCamera->GetRotationMatrix();
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_spReticle->SetActive(true);

		switch (_spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::SitR:
			_spCamera->ChangeSitAimR();
			break;
		case TPSCamera::CameraType::SitL:
			_spCamera->ChangeSitAimL();
			break;
		}
	}
	else
	{
		_spReticle->SetActive(false);

		switch (_spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::SitR:
			_spCamera->ChangeSitR();
			break;
		case TPSCamera::CameraType::SitL:
			_spCamera->ChangeSitL();
			break;
		}
		//_spWeapon->UnHold();

		ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}

	m_mRot = _camRotMat;
}

void Player::ShotBullet(const Math::Vector3& _pos, const Math::Vector3& _dir)
{
	std::shared_ptr<Bullet> bullet;
	bullet = std::make_shared<Bullet>();
	bullet->Init();
	bullet->Shot(_pos, _dir);
	SceneManager::Instance().AddObject(bullet);
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
// 回避処理・・・ここから
//================================================================================================================================
void Player::AvoidProcess()
{
	if (m_posType == PostureType::Stand || m_posType == PostureType::Sit)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			m_pType = PlayerType::Roll;
		}
	}
}
//================================================================================================================================
// 回避処理・・・ここまで
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
		if (!m_posKeyFlg)
		{
			m_posKeyFlg = true;
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
		m_posKeyFlg = false;
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
	const std::shared_ptr<Reticle>	_spReticle = m_wpReticle.lock();

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_shotFlg	= true;
		_spReticle->SetActive(true);
		switch (m_posType)
		{
		case Player::PostureType::Stand:
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
		_spReticle->SetActive(false);

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

void Player::ChangeActionState(std::shared_ptr<ActionStateBase> _nextState)
{
	if (m_nowState)m_nowState->Exit(*this);
	m_nowState = _nextState;
	m_nowState->Enter(*this);
}

void Player::ActionIdle::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Idle");
}

void Player::ActionIdle::Update(Player& _owner)
{
	_owner.StateIdleProc();
}

void Player::ActionIdle::Exit(Player& _owner)
{
}

void Player::ActionWalk::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Walk");
}

void Player::ActionWalk::Update(Player& _owner)
{
	_owner.StateWalkProc();
}

void Player::ActionWalk::Exit(Player& _owner)
{
}

void Player::ActionRun::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Run");
}

void Player::ActionRun::Update(Player& _owner)
{
	_owner.StateRunProc();
}

void Player::ActionRun::Exit(Player& _owner)
{
}

void Player::ActionReady::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Ready");
}

void Player::ActionReady::Update(Player& _owner)
{
	_owner.StateReadyProc();
}

void Player::ActionReady::Exit(Player& _owner)
{
}

void Player::ActionShot::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Shot");
	_owner.m_shotCnt = 0;
}

void Player::ActionShot::Update(Player& _owner)
{
	_owner.StateShotProc();
}

void Player::ActionShot::Exit(Player& _owner)
{
}

void Player::ActionSit::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Sit");
	_owner.m_sitCnt = 0;
	_owner.m_posType = PostureType::Sit;
}

void Player::ActionSit::Update(Player& _owner)
{
	_owner.StateSitProc();
}

void Player::ActionSit::Exit(Player& _owner)
{
}

void Player::ActionStand::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Stand");
	_owner.m_standCnt = 0;
	_owner.m_posType = PostureType::Stand;
}

void Player::ActionStand::Update(Player& _owner)
{
	_owner.StateStandProc();
}

void Player::ActionStand::Exit(Player& _owner)
{
}

void Player::ActionSit_Idle::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Sit_Idle");
}

void Player::ActionSit_Idle::Update(Player& _owner)
{
	_owner.StateSitIdleProc();
}

void Player::ActionSit_Idle::Exit(Player& _owner)
{
}

void Player::ActionSit_Walk::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Sit_Walk");
}

void Player::ActionSit_Walk::Update(Player& _owner)
{
	_owner.StateSitWalkProc();
}

void Player::ActionSit_Walk::Exit(Player& _owner)
{
}

void Player::ActionSit_Ready::Enter(Player& _owner)
{
	_owner.ChanegeAnimation("Sit_Ready");
}

void Player::ActionSit_Ready::Update(Player& _owner)
{
	_owner.StateSitReadyProc();
}

void Player::ActionSit_Ready::Exit(Player& _owner)
{
}
