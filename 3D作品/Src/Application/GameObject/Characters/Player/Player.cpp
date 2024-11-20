#include "Player.h"

#include "Player_UpperBody/Player_UpperBody.h"
#include "Player_LowerBody/Player_LowerBody.h"
#include "Player_Disarm/Player_Disarm.h"
#include "Player_Disarm_Pistol/Player_Disarm_Pistol.h"
#include "Player_Ready_Pistol/Player_Ready_Pistol.h"

#include "../../Camera/TPSCamera/TPSCamera.h"

#include "../../Weapon/Pistol/Pistol_Disarm/Pistol_Disarm.h"
#include "../../Weapon/Pistol/Pistol_Ready/Pistol_Ready.h"

#include "../../Gimmicks/LockedDoor/LockedDoor.h"

#include "../../Item/CardKey/CardKey.h"
#include "../../Item/SecretFile/SecretFile.h"

#include "../../Goal/Goal.h"

#include "../../UI/Reticle/Reticle.h"
#include "../../Bullet/Bullet.h"

#include "../../../Scene/SceneManager.h"

void Player::Init()
{
	//CharaBase::SetAsset("Asset/Models/Characters/Player/Player_3.gltf");

	m_spAnimator	= std::make_shared<KdAnimator>();
	m_nowAnimeFrm	= 0.0f;

	ChangeActionState(std::make_shared<ActionIdle>());

	m_HP			= MAXHP;

	m_pos			= { 0.0f,-0.9f,-50.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.0f;

	m_moveFlg		= false;
	m_shotKeyFlg	= false;
	m_reloadKeyFlg	= false;
	m_holdFlg		= false;
	m_keyFlg		= false;
	m_changeKeyFlg	= false;
	m_posKeyFlg		= false;
	m_creepKeyFlg	= false;

	m_sType			= SituationType::Idle;
	m_posType		= PostureType::Stand;
	m_itemCollType	= ItemCollectType::NoCollect;

	m_objectType	= ObjectType::TypePlayer;

	CharaBase::Init();

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollsion", m_spModel, KdCollider::TypeEvent | KdCollider::TypeBump);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Update()
{
	m_gravity += 0.04f;
	m_pos.y -= m_gravity;

	// 視点切り替え処理
	ChanegeViewPointProcess();

	// 各種「状態」に応じた更新処理を実行する
	if (m_nowState)
	{
		m_nowState->Update(*this);
	}

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;

	//m_disarmPos = m_mUnHold.Translation();

	//KdCollider::SphereInfo disarmSphereInfo;
	//disarmSphereInfo.m_sphere.Center = m_disarmPos;
	//disarmSphereInfo.m_sphere.Radius = 0.05f;
	//disarmSphereInfo.m_type = KdCollider::TypeDamage;

	//m_pDebugWire->AddDebugSphere(disarmSphereInfo.m_sphere.Center, disarmSphereInfo.m_sphere.Radius, kBlueColor);

	//m_readyPos = m_mHold.Translation();

	//KdCollider::SphereInfo readySphereInfo;
	//readySphereInfo.m_sphere.Center = m_readyPos;
	//readySphereInfo.m_sphere.Radius = 0.05f;
	//readySphereInfo.m_type = KdCollider::TypeDamage;

	//m_pDebugWire->AddDebugSphere(readySphereInfo.m_sphere.Center, readySphereInfo.m_sphere.Radius, kBlueColor);

	std::shared_ptr<CardKey>	spCard = m_wpCard.lock();
	std::shared_ptr<LockedDoor>	spLockDoor = m_wpLockDoor.lock();
	std::shared_ptr<SecretFile> spFile = m_wpFile.lock();
	std::shared_ptr<Goal>		spGoal = m_wpGoal.lock();
	if (GetAsyncKeyState('E') & 0x8000)
	{
		if (!m_keyFlg)
		{
			m_keyFlg = true;
			if (spCard)
			{
				if (spCard->GetCollectFlg())
				{
					m_itemCollType = ItemCollectType::CardKeyCollect;
					spCard->Extinction();
				}
			}
			if (spLockDoor)
			{
				if (spLockDoor->GetOpeAbleFlg())
				{
					spLockDoor->Open();
				}
			}
			if (spFile)
			{
				if (spFile->GetCollectFlg())
				{
					m_itemCollType = ItemCollectType::SecretFileCollect;
					spFile->Extinction();
					if (spGoal)
					{
						spGoal->CanBeGoal();
					}
				}
			}
		}
	}
	else
	{
		m_keyFlg = false;
	}

	//m_pDebugWire->AddDebugSphere(readySphereInfo.m_sphere.Center, readySphereInfo.m_sphere.Radius, kBlueColor);
}

void Player::PostUpdate()
{
	UpdateCollision();
	// アニメーションの更新
	//m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	//m_spModel->CalcNodeMatrices();

	//CharaBase::PostUpdate();
}

void Player::UpdateCollision()
{
	//==========================================================
	// レイ判定
	//==========================================================

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = m_pos;
	rayInfo.m_dir = Math::Vector3::Down;
	rayInfo.m_pos.y += 0.01f;

	float enableStepHigh = 0.2f;
	rayInfo.m_pos.y += enableStepHigh;

	rayInfo.m_range = m_gravity + enableStepHigh;
	rayInfo.m_type = KdCollider::TypeGround;

	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}

	float maxOverLap = 0;
	Math::Vector3 hitPos;
	bool isHit = false;
	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			isHit = true;
		}
	}
	if (isHit)
	{
		m_pos = hitPos + Math::Vector3{ 0.f,-0.01f,0.f };
		m_gravity = 0;
	}

	//==========================================================
	// 球判定
	//==========================================================

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center	= m_pos + Math::Vector3{ 0.f,0.9f,0.f };
	sphere.m_sphere.Radius	= 0.3f;
	sphere.m_type			= KdCollider::TypeGround;
	// デバッグ用
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

	// 弾が当たったオブジェクトの情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	// 弾と当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphere, &retSphereList))
		{

		}
	}

	// 球リストから一番近いオブジェクトを検出
	maxOverLap	= 0.f;
	isHit		= false;
	Math::Vector3 hitDir;
	for (auto& ret : retSphereList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap	= ret.m_overlapDistance;
			hitDir		= ret.m_hitDir;
			isHit		= true;
		}
	}
	if (isHit)
	{
		hitDir.Normalize();

		m_pos += hitDir * maxOverLap;
	}
}

//================================================================================================================================
// アニメーション切り替え処理
//================================================================================================================================
void Player::ChangeUpperBodyAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	std::shared_ptr<Player_UpperBody>		spPlayer_Up = m_wpPlayer_Up.lock();
	std::shared_ptr<Player_Ready_Pistol>	spPlayer_Ready_Pistol = m_wpPlayer_Ready_Pistol.lock();
	if (spPlayer_Up)
	{
		spPlayer_Up->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spPlayer_Ready_Pistol)
	{
		spPlayer_Ready_Pistol->ChangeAnimation(_animeName, _isLoop, _time);
	}
}

void Player::ChangeLowerBodyAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	std::shared_ptr<Player_LowerBody>		spPlayer_Low = m_wpPlayer_Low.lock();
	std::shared_ptr<Player_Disarm>			spPlayer_Disarm = m_wpPlayer_Disarm.lock();
	std::shared_ptr<Player_Disarm_Pistol>	spPlayer_Disarm_Pistol = m_wpPlayer_Disarm_Pistol.lock();
	if (spPlayer_Low)
	{
		spPlayer_Low->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spPlayer_Disarm)
	{
		spPlayer_Disarm->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spPlayer_Disarm_Pistol)
	{
		spPlayer_Disarm_Pistol->ChangeAnimation(_animeName, _isLoop, _time);
	}
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
// 視点切り替え処理
//================================================================================================================================
void Player::ChanegeViewPointProcess()
{
	const std::shared_ptr<TPSCamera> spCamera = m_wpCamera.lock();

	if (GetAsyncKeyState('V') & 0x8000)
	{
		switch (spCamera->GetCamType())
		{
		case TPSCamera::CameraType::TpsR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				spCamera->ChangeTPSL();
			}
			break;
		case TPSCamera::CameraType::TpsL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				spCamera->ChangeTPSR();
			}
			break;
		case TPSCamera::CameraType::AimR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				spCamera->ChangeAimL();
			}
			break;
		case TPSCamera::CameraType::AimL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				spCamera->ChangeAimR();
			}
			break;
		case TPSCamera::CameraType::SitR:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				spCamera->ChangeSitL();
			}
			break;
		case TPSCamera::CameraType::SitL:
			if (!m_changeKeyFlg)
			{
				m_changeKeyFlg = true;
				spCamera->ChangeSitR();
			}
			break;
		}
	}
	else
	{
		m_changeKeyFlg = false;
	}
}

void Player::ChangeActionState(std::shared_ptr<ActionStateBase> _nextState)
{
	if (m_nowState)m_nowState->Exit(*this);
	m_nowState = _nextState;
	m_nowState->Enter(*this);
}
//================================================================================================================================
// ステートパターン管理系
//================================================================================================================================
void Player::ActionIdle::Enter(Player& _owner)
{
	if (_owner.m_holdFlg)_owner.m_holdFlg = false;
	if (_owner.m_nowAnimeFrm >= 52.5f && _owner.m_nowAnimeFrm <= _owner.m_reloadFrmMax)
	{
		if (_owner.m_sType != SituationType::Idle || _owner.m_posType != PostureType::Stand)
		{
			_owner.m_posType = PostureType::Stand;
			_owner.m_sType = SituationType::Idle;
			_owner.ChangeUpperBodyAnimation("Idle", _owner.m_nowAnimeFrm);
			_owner.ChangeLowerBodyAnimation("Idle", _owner.m_nowAnimeFrm);
		}
	}
	else
	{
		_owner.m_nowAnimeFrm = 0.0f;
		if (_owner.m_sType != SituationType::Idle || _owner.m_posType != PostureType::Stand)
		{
			_owner.m_posType = PostureType::Stand;
			_owner.m_sType = SituationType::Idle;
			_owner.ChangeUpperBodyAnimation("Idle");
			_owner.ChangeLowerBodyAnimation("Idle");
		}
	}
}

void Player::ActionIdle::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();

	_owner.m_moveDir = Math::Vector3::Zero;

	// ActionRunに切り替え
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward; }
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left; }
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right; }
	if (_owner.m_moveDir.LengthSquared() > 0)
	{
		_owner.ChangeActionState(std::make_shared<ActionRun>());
		return;
	}

	// ActionReloadに切り替え
	if (GetAsyncKeyState('R') & 0x8000)
	{
		if (!_owner.m_reloadKeyFlg)
		{
			_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
		}
	}

	// ActionSitに切り替え
	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!_owner.m_posKeyFlg)
		{
			_owner.m_posKeyFlg = true;
			_owner.ChangeActionState(std::make_shared<ActionSit>());
			switch (spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		_owner.m_posKeyFlg = false;
	}

	// ActionReadyに切り替え
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionReady>());
		return;
	}

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionIdle::Exit(Player& _owner)
{
}

void Player::ActionWalk::Enter(Player& _owner)
{
	_owner.m_nowAnimeFrm = 0.0f;
	if (_owner.m_sType != SituationType::Walk)
	{
		_owner.m_sType = SituationType::Walk;
		_owner.ChangeUpperBodyAnimation("Walk");
		_owner.ChangeLowerBodyAnimation("Walk");
	}
}

void Player::ActionWalk::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();

	Math::Matrix camRotMat;
	if (spCamera)
	{
		camRotMat = spCamera->GetRotationMatrix();
	}

	_owner.m_moveSpeed = 0.05f;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		_owner.m_moveDir = Math::Vector3::Zero;

		if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward; }
		if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward; }
		if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left; }
		if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right; }
	}
	else
	{
		if (_owner.m_moveDir.LengthSquared() == 0)
		{
			_owner.ChangeActionState(std::make_shared<ActionIdle>());
			return;
		}
		else
		{
			_owner.ChangeActionState(std::make_shared<ActionRun>());
			return;
		}
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!_owner.m_posKeyFlg)
		{
			_owner.m_posKeyFlg = true;
			_owner.ChangeActionState(std::make_shared<ActionSit>());
			switch (spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		_owner.m_posKeyFlg = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionReady>());
	}

	if (spCamera)
	{
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	_owner.m_moveDir.Normalize();
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	//m_mRot = _camRotMat;
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionWalk::Exit(Player& _owner)
{
}

void Player::ActionRun::Enter(Player& _owner)
{
	if (_owner.m_sType != SituationType::Run)
	{
		_owner.m_sType = SituationType::Run;
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Run");
		_owner.ChangeLowerBodyAnimation("Run");
	}
}

void Player::ActionRun::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();

	_owner.m_moveSpeed = 0.12f;

	_owner.m_moveDir = Math::Vector3::Zero;

	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward; }
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left; }
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right; }
	if (_owner.m_moveDir.LengthSquared() == 0)
	{
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		if (_owner.m_posType == PostureType::Stand)
		{
			_owner.ChangeActionState(std::make_shared<ActionWalk>());
			return;
		}
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!_owner.m_posKeyFlg)
		{
			_owner.m_posKeyFlg = true;
			_owner.ChangeActionState(std::make_shared<ActionSit>());
			switch (spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		_owner.m_posKeyFlg = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionReady>());
	}

	if (spCamera)
	{
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	_owner.m_moveDir.Normalize();
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	//m_mRot = _camRotMat;
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionRun::Exit(Player& _owner)
{
}

void Player::ActionReady::Enter(Player& _owner)
{
	if (_owner.m_sType != SituationType::Ready)
	{
		_owner.m_sType = SituationType::Ready;
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Ready");
		_owner.ChangeLowerBodyAnimation("Ready");
	}
}

void Player::ActionReady::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera>	spCamera	= _owner.m_wpCamera.lock();
	const std::shared_ptr<Reticle>		spReticle	= _owner.m_wpReticle.lock();

	Math::Matrix camRotMat;
	if (spCamera)
	{
		camRotMat = spCamera->GetRotationMatrix();
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		spReticle->SetActive(true);

		switch (spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::TpsR:
			spCamera->ChangeAimR();
			break;
		case TPSCamera::CameraType::TpsL:
			spCamera->ChangeAimL();
			break;
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (!_owner.m_shotKeyFlg)
			{
				_owner.m_shotKeyFlg = true;
				_owner.ChangeActionState(std::make_shared<ActionShot>());
			}
		}
		else
		{
			_owner.m_shotKeyFlg = false;
		}
	}
	else
	{
		spReticle->SetActive(false);

		switch (spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::TpsR:
			spCamera->ChangeTPSR();
			break;
		case TPSCamera::CameraType::TpsL:
			spCamera->ChangeTPSL();
			break;
		}
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	_owner.m_mRot = camRotMat;
}

void Player::ActionReady::Exit(Player& _owner)
{
}

void Player::ActionShot::Enter(Player& _owner)
{
	if (_owner.m_sType != SituationType::Shot)
	{
		_owner.m_sType = SituationType::Shot;
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Shot", false);
		_owner.ChangeLowerBodyAnimation("Shot", false);
	}
}

void Player::ActionShot::Update(Player& _owner)
{
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	KdCollider::RayInfo ray;
	ray.m_pos	= _owner.m_pos;
	ray.m_dir	= _owner.m_mWorld.Backward();
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
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
		}
	}

	Math::Vector3 bulletDir = hitPos - _owner.m_pos;

	if (_owner.m_nowAnimeFrm >= 2.5f)
	{
		_owner.ShotBullet(_owner.m_pos, bulletDir);
	}

	if (_owner.m_nowAnimeFrm >= _owner.m_shotFrmMax)
	{
		_owner.ChangeActionState(std::make_shared<ActionReady>());
	}
}

void Player::ActionShot::Exit(Player& _owner)
{
}

void Player::ActionReload_Idle::Enter(Player& _owner)
{
	if (_owner.m_sType != Player::SituationType::Reload)
	{
		_owner.m_sType = Player::SituationType::Reload;
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Reload", false);
		_owner.ChangeLowerBodyAnimation("Reload", false);
	}
	else
	{
		if (_owner.m_nowAnimeFrm != 0.0f)
		{
			_owner.ChangeUpperBodyAnimation("Reload", false, _owner.m_nowAnimeFrm);
			_owner.ChangeLowerBodyAnimation("Reload", false, _owner.m_nowAnimeFrm);
		}
		else
		{
			_owner.m_nowAnimeFrm = 0.0f;
			_owner.ChangeUpperBodyAnimation("Reload", false);
			_owner.ChangeLowerBodyAnimation("Reload", false);
		}
	}
}

void Player::ActionReload_Idle::Update(Player& _owner)
{
	_owner.m_moveDir = Math::Vector3::Zero;

	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;
	// ActionRunに切り替え
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}
	if (_owner.m_moveDir.LengthSquared() > 0)
	{
		_owner.ChangeActionState(std::make_shared<ActionReload_Run>());
		return;
	}

	// アニメーションフレームが最後まで再生されたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		_owner.m_nowAnimeFrm = _owner.m_reloadFrmMax;
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
	}

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionReload_Idle::Exit(Player& _owner)
{
}

void Player::ActionReload_Run::Enter(Player& _owner)
{
	if (_owner.m_sType != Player::SituationType::Reload)
	{
		_owner.m_sType = Player::SituationType::Reload;
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Reload_Run", false);
		_owner.ChangeLowerBodyAnimation("Reload_Run", false);
	}
	else
	{
		if (_owner.m_nowAnimeFrm != 0.0f)
		{
			_owner.ChangeUpperBodyAnimation("Reload_Run", false, _owner.m_nowAnimeFrm);
			_owner.ChangeLowerBodyAnimation("Reload_Run", false, _owner.m_nowAnimeFrm);
		}
		else
		{
			_owner.m_nowAnimeFrm = 0.0f;
			_owner.ChangeUpperBodyAnimation("Reload_Run", false);
			_owner.ChangeLowerBodyAnimation("Reload_Run", false);
		}
	}
}

void Player::ActionReload_Run::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera>	spCamera = _owner.m_wpCamera.lock();

	_owner.m_moveSpeed = 0.12f;

	_owner.m_moveDir = Math::Vector3::Zero;

	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward; }
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left; }
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right; }
	if (_owner.m_moveDir.LengthSquared() == 0)
	{
		// アニメーションフレームが最後まで再生されたら
		_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
		return;
	}

	// アニメーションフレームが最後まで再生されたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		_owner.m_nowAnimeFrm = _owner.m_reloadFrmMax;
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	if (spCamera)
	{
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	_owner.m_moveDir.Normalize();
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	//m_mRot = _camRotMat;
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionReload_Run::Exit(Player& _owner)
{
}

void Player::ActionSit::Enter(Player& _owner)
{
	if (_owner.m_posType != PostureType::Sit)
	{
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Sit", false);
		_owner.ChangeLowerBodyAnimation("Sit", false);
	}
}

void Player::ActionSit::Update(Player& _owner)
{
	// アニメーションフレームが最後まで再生されたら
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;
	if (_owner.m_nowAnimeFrm >= _owner.m_sitFrmMax)
	{
		_owner.ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}
}

void Player::ActionSit::Exit(Player& _owner)
{
}

void Player::ActionStand::Enter(Player& _owner)
{
	if (_owner.m_posType != PostureType::Stand)
	{
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Stand");
		_owner.ChangeLowerBodyAnimation("Stand");
	}
}

void Player::ActionStand::Update(Player& _owner)
{
	// アニメーションフレームが最後まで再生されたら
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;
	if (_owner.m_nowAnimeFrm >= _owner.m_standFrmMax)
	{
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}
}

void Player::ActionStand::Exit(Player& _owner)
{
}

void Player::ActionSit_Idle::Enter(Player& _owner)
{
	if (_owner.m_sType != SituationType::Idle || _owner.m_posType != PostureType::Sit)
	{
		_owner.m_posType		= PostureType::Sit;
		_owner.m_sType			= SituationType::Idle;
		_owner.m_nowAnimeFrm	= 0.0f;
		_owner.ChangeUpperBodyAnimation("Sit_Idle");
		_owner.ChangeLowerBodyAnimation("Sit_Idle");
	}
}

void Player::ActionSit_Idle::Update(Player& _owner)
{
	std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();

	_owner.m_moveDir = Math::Vector3::Zero;

	// ActionSit_Runに切り替え
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward; }
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left; }
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right; }
	if (_owner.m_moveDir.LengthSquared() > 0)
	{
		_owner.ChangeActionState(std::make_shared<ActionSit_Walk>());
		return;
	}

	// ActionStandに切り替え
	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!_owner.m_posKeyFlg)
		{
			_owner.m_posKeyFlg = true;
			_owner.ChangeActionState(std::make_shared<ActionStand>());
			switch (spCamera->GetCamType())
			{
			case TPSCamera::CameraType::SitL:
				spCamera->ChangeTPSL();
				break;
			case TPSCamera::CameraType::SitR:
				spCamera->ChangeTPSR();
				break;
			}
			return;
		}
	}
	else
	{
		_owner.m_posKeyFlg = false;
	}

	// ActionSit_Readyに切り替え
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionSit_Ready>());
		return;
	}

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionSit_Idle::Exit(Player& _owner)
{
}

void Player::ActionSit_Walk::Enter(Player& _owner)
{
	if (_owner.m_sType != SituationType::Walk)
	{
		_owner.m_sType = SituationType::Walk;
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Sit_Walk");
		_owner.ChangeLowerBodyAnimation("Sit_Walk");
	}
}

void Player::ActionSit_Walk::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();

	_owner.m_moveSpeed = 0.08f;

	_owner.m_moveDir = Math::Vector3::Zero;

	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward; }
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward; }
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left; }
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right; }
	if (_owner.m_moveDir.LengthSquared() == 0)
	{
		_owner.ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}

	if (GetAsyncKeyState('C') & 0x8000)
	{
		if (!_owner.m_posKeyFlg)
		{
			_owner.m_posKeyFlg = true;
			_owner.ChangeActionState(std::make_shared<ActionStand>());
			switch (spCamera->GetCamType())
			{
			case TPSCamera::CameraType::TpsL:
				spCamera->ChangeSitL();
				break;
			case TPSCamera::CameraType::TpsR:
				spCamera->ChangeSitR();
				break;
			}
			return;
		}
	}
	else
	{
		_owner.m_posKeyFlg = false;
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionSit_Ready>());
	}

	if (spCamera)
	{
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	_owner.m_moveDir.Normalize();
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	//m_mRot = _camRotMat;
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionSit_Walk::Exit(Player& _owner)
{
}

void Player::ActionSit_Ready::Enter(Player& _owner)
{
	if (_owner.m_sType != SituationType::Ready)_owner.m_sType = SituationType::Ready;
	_owner.m_nowAnimeFrm = 0.0f;
	_owner.ChangeUpperBodyAnimation("Sit_Ready");
	_owner.ChangeLowerBodyAnimation("Sit_Ready");
}

void Player::ActionSit_Ready::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();
	const std::shared_ptr<Reticle>	spReticle = _owner.m_wpReticle.lock();

	Math::Matrix camRotMat;
	if (spCamera)
	{
		camRotMat = spCamera->GetRotationMatrix();
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		spReticle->SetActive(true);

		switch (spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::SitR:
			spCamera->ChangeSitAimR();
			break;
		case TPSCamera::CameraType::SitL:
			spCamera->ChangeSitAimL();
			break;
		}
	}
	else
	{
		spReticle->SetActive(false);

		switch (spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::SitR:
			spCamera->ChangeSitR();
			break;
		case TPSCamera::CameraType::SitL:
			spCamera->ChangeSitL();
			break;
		}
		//_spWeapon->UnHold();

		_owner.ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}

	_owner.m_mRot = camRotMat;
}

void Player::ActionSit_Ready::Exit(Player& _owner)
{
}
//================================================================================================================================
// ステートパターン管理系
//================================================================================================================================
