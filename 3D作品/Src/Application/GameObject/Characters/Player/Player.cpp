#include "Player.h"

#include "Player_UpperBody/Player_UpperBody.h"
#include "Player_LowerBody/Player_LowerBody.h"
#include "Player_Disarm/Player_Disarm.h"
#include "Player_Disarm_Pistol/Player_Disarm_Pistol.h"
#include "Player_Ready_Pistol/Player_Ready_Pistol.h"

#include "../../Camera/TPSCamera/TPSCamera.h"

#include "../../Gimmicks/LockedDoor/LockedDoor.h"

#include "../../Item/CardKey/CardKey.h"
#include "../../Item/SecretFile/SecretFile.h"

#include "../../Goal/Goal.h"

#include "../../UI/Reticle/Reticle.h"
#include "../../UI/CollectCardKeyUI/CollectCardKeyUI.h"
#include "../../UI/CollectSecretFileUI/CollectSecretFileUI.h"
#include "../../UI/CollectUIBack/CollectUIBack.h"
#include "../../UI/MiniMapUI/MiniMapUI.h"
#include "../../UI/MiniMapUIBack/MiniMapUIBack.h"
#include "../../UI/CurrentLocation/CurrentLocation.h"

#include "../../Bullet/Bullet.h"

#include "../../../Scene/SceneManager.h"
#include "../../../main.h"

void Player::Init()
{
	m_spAnimator	= std::make_shared<KdAnimator>();
	m_nowAnimeFrm	= 0.0f;

	ChangeActionState(std::make_shared<ActionIdle>());

	m_pos			= { 0.0f,-0.9f,-50.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.0f;

	m_openMapKeyFlg = false;
	m_shotKeyFlg	= false;
	m_reloadKeyFlg	= false;
	m_keyFlg		= false;
	m_changeKeyFlg	= false;
	m_posKeyFlg		= false;

	m_sType			= SituationType::Idle;
	m_posType		= PostureType::Stand;
	m_itemCollType	= ItemCollectType::NoCollect;

	m_objectType	= KdGameObject::ObjectType::TypePlayer;

	CharaBase::Init();

	m_pDebugWire	= std::make_unique<KdDebugWireFrame>();
}

void Player::Update()
{
	m_gravity += 0.04f;
	m_pos.y -= m_gravity;

	// 視点切り替え処理
	ChanegeViewPointProc();

	// マップ展開処理
	OpneMapProc();

	// アイテム回収処理
	CollectItemProc();

	// 各種「状態」に応じた更新処理を実行する
	if (m_nowState)
	{
		m_nowState->Update(*this);
	}

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;

	//m_pDebugWire->AddDebugSphere(readySphereInfo.m_sphere.Center, readySphereInfo.m_sphere.Radius, kBlueColor);
}

void Player::PostUpdate()
{
	// 当たり判定(地面判定はレイ判定、衝突判定は球判定)
	UpdateCollision();
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
// マップ展開処理
//================================================================================================================================
void Player::OpneMapProc()
{
	const std::shared_ptr<MiniMapUI>		spMiniMapUI = m_wpMiniMapUI.lock();
	const std::shared_ptr<MiniMapUIBack>	spMiniMapUIBack = m_wpMiniMapUIBack.lock();
	const std::shared_ptr<CurrentLocation>	spCurrentLocation = m_wpCurrentLocation.lock();

	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		if (!m_openMapKeyFlg)
		{
			m_openMapKeyFlg = true;

			if (spMiniMapUI)
			{
				if (!spMiniMapUI->GetActive())
				{
					spMiniMapUI->Open(true);
				}
				else
				{
					spMiniMapUI->Open(false);
				}
			}

			if (spMiniMapUIBack)
			{
				if (!spMiniMapUIBack->GetActive())
				{
					spMiniMapUIBack->Open(true);
				}
				else
				{
					spMiniMapUIBack->Open(false);
				}
			}

			if (spCurrentLocation)
			{
				if (!spCurrentLocation->GetActive())
				{
					spCurrentLocation->Open(true);
				}
				else
				{
					spCurrentLocation->Open(false);
				}
			}
		}
	}
	else
	{
		m_openMapKeyFlg = false;
	}
}

//================================================================================================================================
// アイテム回収処理
//================================================================================================================================
void Player::CollectItemProc()
{
	const std::shared_ptr<CardKey>			spCard		= m_wpCard.lock();
	const std::shared_ptr<LockedDoor>		spLockDoor	= m_wpLockDoor.lock();
	const std::shared_ptr<SecretFile>		spFile		= m_wpFile.lock();
	const std::shared_ptr<Goal>				spGoal		= m_wpGoal.lock();

	// Eキーが押されたとき
	if (GetAsyncKeyState('E') & 0x8000)
	{
		// キーフラグがfalseのとき
		if (!m_keyFlg)
		{
			// キーフラグをtrueにする
			m_keyFlg = true;

			// カードキーの情報があるとき
			if (spCard)
			{
				// カードキー回収フラグがtrueのとき
				if (spCard->GetCollectFlg())
				{
					// カードキー入手時のUIを生成
					std::shared_ptr<CollectUIBack> collectUIBack;
					collectUIBack = std::make_shared<CollectUIBack>();
					collectUIBack->Init();
					SceneManager::Instance().AddObject(collectUIBack);

					std::shared_ptr<CollectCardKeyUI> collectCardUI;
					collectCardUI = std::make_shared<CollectCardKeyUI>();
					collectCardUI->Init();
					SceneManager::Instance().AddObject(collectCardUI);

					// アイテム取得状態を"カードキー取得状態"にし、CardKeyクラスを破棄する
					m_itemCollType = ItemCollectType::CardKeyCollect;
					spCard->Extinction();
				}
			}
			// 鍵のかかったドアの情報があるとき
			if (spLockDoor)
			{
				// ドア開錠可能フラグがtrueのとき
				if (spLockDoor->GetOpeAbleFlg())
				{
					// ドアを開錠する
					spLockDoor->Open();
				}
			}
			// 機密ファイルの情報があるとき
			if (spFile)
			{

				// 機密ファイル回収フラグがtrueのとき
				if (spFile->GetCollectFlg())
				{
					// 機密ファイル入手時のUIを生成
					std::shared_ptr<CollectUIBack> collectUIBack;
					collectUIBack = std::make_shared<CollectUIBack>();
					collectUIBack->Init();
					SceneManager::Instance().AddObject(collectUIBack);

					std::shared_ptr<CollectSecretFileUI> collectFileUI;
					collectFileUI = std::make_shared<CollectSecretFileUI>();
					collectFileUI->Init();
					SceneManager::Instance().AddObject(collectFileUI);

					// アイテム取得状態を"機密ファイル取得状態"にし、SecretFileクラスを破棄する
					m_itemCollType = ItemCollectType::SecretFileCollect;
					spFile->Extinction();

					// ゴールの情報があるとき
					if (spGoal)
					{
						// ゴール可能状態にする
						spGoal->CanBeGoal();
					}
				}
			}
		}
	}
	// キーが離されたとき
	else
	{
		// キーフラグをfalseにする
		m_keyFlg = false;
	}
}

//================================================================================================================================
// 自動リロード処理
//================================================================================================================================
void Player::AutoReloadProc()
{
	const std::shared_ptr<Player_Ready_Pistol> spPlayer_Ready_Pistol = m_wpPlayer_Ready_Pistol.lock();
	UINT nowBullet = 0;

	// 銃の情報があるとき
	if (spPlayer_Ready_Pistol)
	{
		// 残弾数を取得
		nowBullet = spPlayer_Ready_Pistol->GetNowBullet();

		// 残弾数が空のとき
		if (nowBullet <= spPlayer_Ready_Pistol->GetMagazinEmpty())
		{
			switch (m_posType)
			{
			case Player::PostureType::Stand:

				// Reload_Idleに切り替え
				ChangeActionState(std::make_shared<ActionReload_Idle>());
				return;
				break;
			case Player::PostureType::Sit:
				break;
			case Player::PostureType::Creep:
				break;
			default:
				break;
			}
		}
	}
}

//================================================================================================================================
// アニメーション切り替え処理
//================================================================================================================================

// プレイヤー上半身のアニメーション切り替え処理
void Player::ChangeUpperBodyAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	const std::shared_ptr<Player_UpperBody>		spPlayer_Up				= m_wpPlayer_Up.lock();
	const std::shared_ptr<Player_Ready_Pistol>	spPlayer_Ready_Pistol	= m_wpPlayer_Ready_Pistol.lock();
	if (spPlayer_Up)
	{
		spPlayer_Up->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spPlayer_Ready_Pistol)
	{
		spPlayer_Ready_Pistol->ChangeAnimation(_animeName, _isLoop, _time);
	}
}

// プレイヤー下半身のアニメーション切り替え処理
void Player::ChangeLowerBodyAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	const std::shared_ptr<Player_LowerBody>		spPlayer_Low			= m_wpPlayer_Low.lock();
	const std::shared_ptr<Player_Disarm>		spPlayer_Disarm			= m_wpPlayer_Disarm.lock();
	const std::shared_ptr<Player_Disarm_Pistol>	spPlayer_Disarm_Pistol	= m_wpPlayer_Disarm_Pistol.lock();
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

//================================================================================================================================
// 視点切り替え処理
//================================================================================================================================
void Player::ChanegeViewPointProc()
{
	const std::shared_ptr<TPSCamera> spCamera = m_wpCamera.lock();

	// Vキーが押されたとき
	if (GetAsyncKeyState('V') & 0x8000)
	{
		// カメラの位置を切り替え(複数の種類があるためswitch文で判定)
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
	// キーが離されたとき
	else
	{
		// 視点切り替えキーフラグをfalseにする
		m_changeKeyFlg = false;
	}
}

// ステート切り替え処理
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
	// リロード時の現在のアニメーションフレームが52.5f以上かつ60.0f未満のとき
	if (_owner.m_nowAnimeFrm >= 52.5f && _owner.m_nowAnimeFrm <= _owner.m_reloadFrmMax)
	{
		// プレイヤーの状態が"停止状態"またはプレイヤーの姿勢状態が"立ち状態"でないとき
		if (_owner.m_sType != SituationType::Idle || _owner.m_posType != PostureType::Stand)
		{
			// プレイヤーの状態を"停止状態"にし、プレイヤーの姿勢状態を"立ち状態"にする
			_owner.m_posType = PostureType::Stand;
			_owner.m_sType = SituationType::Idle;
			// プレイヤーの上半身と下半身のアニメーションを"Idle"に切り替え
			_owner.ChangeUpperBodyAnimation("Idle", _owner.m_nowAnimeFrm);
			_owner.ChangeLowerBodyAnimation("Idle", _owner.m_nowAnimeFrm);
		}
	}
	else
	{
		// 現在のアニメーションフレームを初期化
		_owner.m_nowAnimeFrm = 0.0f;
		// プレイヤーの状態が"停止状態"またはプレイヤーの姿勢状態が"立ち状態"でないとき
		if (_owner.m_sType != SituationType::Idle || _owner.m_posType != PostureType::Stand)
		{
			// プレイヤーの状態を"停止状態"にし、プレイヤーの姿勢状態を"立ち状態"にする
			_owner.m_posType = PostureType::Stand;
			_owner.m_sType = SituationType::Idle;
			// プレイヤーの上半身と下半身のアニメーションを"Idle"に切り替え
			_owner.ChangeUpperBodyAnimation("Idle");
			_owner.ChangeLowerBodyAnimation("Idle");
		}
	}
}

void Player::ActionIdle::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();

	// プレイヤーの移動方向を常に初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// 各移動キーを押されたとき、ActionRunに切り替え
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}
	if (_owner.m_moveDir.LengthSquared() > 0)
	{
		_owner.ChangeActionState(std::make_shared<ActionRun>());
		return;
	}

	// 自動リロード処理
	_owner.AutoReloadProc();

	// 銃の情報があるとき
	if (spReady_Pistol)
	{
		// "R"キーを押されたとき
		if (GetAsyncKeyState('R') & 0x8000)
		{
			// 残弾数がマガジンサイズ以下のとき
			if (spReady_Pistol->GetNowBullet() <= spReady_Pistol->GetMagazinSize())
			{
				// リロードキーフラグがfalseのとき
				if (!_owner.m_reloadKeyFlg)
				{
					// リロードフラグをtrueにし、ActionReload_Idleに切り替え
					_owner.m_reloadKeyFlg = true;
					_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
				}
			}
		}
		// キーが離されたとき
		else
		{
			_owner.m_reloadKeyFlg = false;
		}
	}

	// "C"キーを押したとき
	if (GetAsyncKeyState('C') & 0x8000)
	{
		// 姿勢状態切り替えキーフラグがfalseのとき
		if (!_owner.m_posKeyFlg)
		{
			// 姿勢状態切り替えキーフラグをtrueにし、
			// カメラの状態を切り替え、ActionSitに切り替える
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
	// キーが離されたとき
	else
	{
		// 姿勢状態切り替えキーフラグ
		_owner.m_posKeyFlg = false;
	}

	// 自動リロード処理
	_owner.AutoReloadProc();

	// マウス右クリックされたとき、ActionReadyに切り替える
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionReady>());
		return;
	}

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionIdle::Exit(Player& _owner)
{
	// 現在のアニメーションフレームを初期化する
	_owner.m_nowAnimeFrm = 0.0f;
}

void Player::ActionWalk::Enter(Player& _owner)
{
	// プレイヤーの状態が"歩行状態"でないとき
	if (_owner.m_sType != SituationType::Walk)
	{
		// プレイヤーの状態を"歩行状態"にする
		_owner.m_sType = SituationType::Walk;
		// プレイヤーの上半身と下半身のアニメーションを"Walk"に切り替え
		_owner.ChangeUpperBodyAnimation("Walk");
		_owner.ChangeLowerBodyAnimation("Walk");
	}
}

void Player::ActionWalk::Update(Player& _owner)
{
	// カメラ情報取得
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();

	// 現在の移動速度が歩行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_walkMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_walkMoveSpd;
	}

	// CTRLキーが押されたとき
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		// 移動方向を常に初期化
		_owner.m_moveDir = Math::Vector3::Zero;

		// 各移動キーが押されたとき、対応した方向に向ける
		if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
		if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
		if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
		if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}
	}
	// キーが離されたとき
	else
	{
		// 移動方向のベクトルの長さが0のとき、ActionIdleに切り替え
		if (_owner.m_moveDir.LengthSquared() == 0)
		{
			_owner.ChangeActionState(std::make_shared<ActionIdle>());
			return;
		}
		// そうでないとき、ActionRunに切り替え
		else
		{
			_owner.ChangeActionState(std::make_shared<ActionRun>());
			return;
		}
	}

	// 自動リロード処理
	_owner.AutoReloadProc();

	// 銃の情報があるとき
	if (spReady_Pistol)
	{
		// "Rキーが押されたとき"
		if (GetAsyncKeyState('R') & 0x8000)
		{
			// 残弾数がマガジンサイズ以下のとき
			if (spReady_Pistol->GetNowBullet() <= spReady_Pistol->GetMagazinSize())
			{
				// リロードキーフラグがfalseのとき
				if (!_owner.m_reloadKeyFlg)
				{
					// リロードキーフラグをtrueにし、ActionReload_Walkに切り替え
					_owner.m_reloadKeyFlg = true;
					_owner.ChangeActionState(std::make_shared<ActionReload_Walk>());
					return;
				}
			}
		}
		// キーフラグが離されたとき
		else
		{
			// リロードキーフラグをfalseにする
			_owner.m_reloadKeyFlg = false;
		}
	}

	// "C"キーが押されたとき
	if (GetAsyncKeyState('C') & 0x8000)
	{
		// 姿勢状態切り替えキーフラグがfalseのとき
		if (!_owner.m_posKeyFlg)
		{
			// 姿勢状態切り替えキーフラグをtrueにし、
			// カメラの状態を切り替え、ActionSitに切り替え
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
	// キーが離されたとき
	else
	{
		// 姿勢状態切り替えキーフラグをfalseにする
		_owner.m_posKeyFlg = false;
	}

	// マウス右クリックされたとき、ActionReadyに切り替え 
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionReady>());
	}

	// カメラ情報があるとき
	if (spCamera)
	{
		// 移動方向の法線ベクトルをカメラの回転行列に変換
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	// 移動方向正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionWalk::Exit(Player& _owner)
{
	// 現在のアニメーションフレームを初期化する
	_owner.m_nowAnimeFrm = 0.0f;
}

void Player::ActionRun::Enter(Player& _owner)
{
	// プレイヤーの状態が"走行状態"でないとき
	if (_owner.m_sType != SituationType::Run)
	{
		// プレイヤーの状態を"走行状態"に切り替えて、
		// プレイヤーの上半身と下半身のアニメーションを"Run"に切り替え
		_owner.m_sType = SituationType::Run;
		_owner.ChangeUpperBodyAnimation("Run");
		_owner.ChangeLowerBodyAnimation("Run");
	}
}

void Player::ActionRun::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();

	// 現在の移動速度が走行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_runMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_runMoveSpd;
	}

	// 移動方向初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// 各移動キーが押されたとき、対応した方向に向ける
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}

	// 移動方向のベクトルの長さが0でないとき、ActionIdleに切り替え
	if (_owner.m_moveDir.LengthSquared() == 0)
	{
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	// CTRLキーが押されたとき
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		// プレイヤーの姿勢状態が"立ち状態"であるとき、ActionWalkに切り替え
		if (_owner.m_posType == PostureType::Stand)
		{
			_owner.ChangeActionState(std::make_shared<ActionWalk>());
			return;
		}
	}

	// 自動リロード処理
	_owner.AutoReloadProc();

	// 銃の情報があるとき
	if (spReady_Pistol)
	{
		// "R"キーが押されたとき
		if (GetAsyncKeyState('R') & 0x8000)
		{
			// 残弾数がマガジンサイズ以下のとき
			if (spReady_Pistol->GetNowBullet() <= spReady_Pistol->GetMagazinSize())
			{
				// リロードキーフラグがfalseのとき
				if (!_owner.m_reloadKeyFlg)
				{
					// リロードキーフラグをtrueにし、ActionReload_Runに切り替え
					_owner.m_reloadKeyFlg = true;
					_owner.ChangeActionState(std::make_shared<ActionReload_Run>());
				}
			}
		}
		// 離されたとき
		else
		{
			// リロードキーフラグをfalseにする
			_owner.m_reloadKeyFlg = false;
		}
	}

	// "C"キーが押されたとき
	if (GetAsyncKeyState('C') & 0x8000)
	{
		// 姿勢状態切り替えフラグがfalseのとき
		if (!_owner.m_posKeyFlg)
		{
			// 姿勢状態切り替えキーフラグをtrueにし、
			// カメラの状態を切り替え、ActionSitに切り替え
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
	// 離されたとき
	else
	{
		// 姿勢状態切り替えフラグをfalseにする
		_owner.m_posKeyFlg = false;
	}

	// マウス右クリックされたとき、ActionReadyに切り替え
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionReady>());
	}

	// カメラ情報があるとき
	if (spCamera)
	{
		// 移動方向の法線ベクトルをカメラの回転行列に変換
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionRun::Exit(Player& _owner)
{
	// 現在のアニメーションフレームを初期化する
	_owner.m_nowAnimeFrm = 0.0f;
}

void Player::ActionReady::Enter(Player& _owner)
{
	// プレイヤーの状態が"構え状態"でないとき
	if (_owner.m_sType != SituationType::Ready)
	{
		// プレイヤーの状態を"構え状態"に切り替えて、
		// プレイヤーの上半身と下半身のアニメーションを"Ready"に切り替え
		_owner.m_sType = SituationType::Ready;
		_owner.ChangeUpperBodyAnimation("Ready");
		_owner.ChangeLowerBodyAnimation("Ready");
	}
}

void Player::ActionReady::Update(Player& _owner)
{
	// カメラ情報とレティクル情報取得
	const std::shared_ptr<TPSCamera>	spCamera	= _owner.m_wpCamera.lock();
	const std::shared_ptr<Reticle>		spReticle	= _owner.m_wpReticle.lock();

	// カメラの行列を格納するためにローカル行列を宣言
	Math::Matrix camRotMat;

	// カメラの情報があれば、カメラの行列をローカル行列に格納
	if (spCamera)
	{
		camRotMat = spCamera->GetRotationMatrix();
	}

	// 現在の移動速度が走行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_runMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_runMoveSpd;
	}

	// 移動方向初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// 各移動キーが押されたとき、対応した方向に向ける
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}

	// 右クリックされたとき
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// レティクルを表示し、カメラの状態を切り替え
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

		// 左クリックされたとき
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			// 発射キーフラグがfalseのとき
			if (!_owner.m_shotKeyFlg)
			{
				// 発射キーフラグをtrueにし、ActionShotに切り替え
				_owner.m_shotKeyFlg = true;
				_owner.ChangeActionState(std::make_shared<ActionShot>());
			}
		}
		// クリックが離されたとき
		else
		{
			// 発射キーフラグをfalseにする
			_owner.m_shotKeyFlg = false;
		}
	}
	// クリックが離されたとき
	else
	{
		// レティクルを非表示にし、カメラの状態を切り替え、
		// ActionIdleに切り替え
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

	// 自動リロード処理
	_owner.AutoReloadProc();

	// "R"キーを押されたとき
	if (GetAsyncKeyState('R') & 0x8000)
	{
		// リロードキーフラグがfalseのとき
		if (!_owner.m_reloadKeyFlg)
		{
			// リロードキーフラグをtrueにし、ActionReload_Idleに切り替え
			_owner.m_reloadKeyFlg = true;
			_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
		}
	}
	// キーが離されたとき
	else
	{
		// リロードキーフラグをfalseにする
		_owner.m_reloadKeyFlg = false;
	}

	// ベクトルの向きをY軸の回転行列で変換
	_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	// カメラのローカル回転行列を格納
	_owner.m_mRot = camRotMat;
}

void Player::ActionReady::Exit(Player& _owner)
{
	// 現在のアニメーションフレームを初期化
	_owner.m_nowAnimeFrm = 0.0f;
}

void Player::ActionShot::Enter(Player& _owner)
{
	// プレイヤーの状態が"発射状態"でないとき
	if (_owner.m_sType != SituationType::Shot)
	{
		// プレイヤーの状態を"発射状態"にし、
		// 上半身と下半身のアニメーションを"Shot"に切り替え
		_owner.m_sType = SituationType::Shot;
		_owner.ChangeUpperBodyAnimation("Shot", false);
		_owner.ChangeLowerBodyAnimation("Shot", false);
	}
}

void Player::ActionShot::Update(Player& _owner)
{
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();

	// カメラの行列を格納するためにローカル行列を宣言
	Math::Matrix camRotMat;

	// カメラの情報があれば、カメラの行列をローカル行列に格納
	if (spCamera)
	{
		camRotMat = spCamera->GetRotationMatrix();
	}

	// 現在の移動速度が走行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_runMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_runMoveSpd;
	}

	// 移動方向初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// 各移動キーが押されたとき、対応した方向に向ける
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}

	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// 現在のアニメーションフレームが弾発射アニメーションフレーム以上のとき
	if (_owner.m_nowAnimeFrm >= _owner.m_bulletShotFrm)
	{
		spReady_Pistol->ShotBullet(true);
	}

	// 現在のアニメーションフレームが最後まで再生されたら
	if (_owner.m_nowAnimeFrm >= _owner.m_shotFrmMax)
	{
		// ActionReadyに切り替え
		_owner.ChangeActionState(std::make_shared<ActionReady>());
	}

	// ベクトルの向きをY軸の回転行列で変換
	_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	// カメラのローカル回転行列を格納
	_owner.m_mRot = camRotMat;
}

void Player::ActionShot::Exit(Player& _owner)
{
	const std::shared_ptr<Player_Ready_Pistol> spPlayer_Ready_Pistol = _owner.m_wpPlayer_Ready_Pistol.lock();

	spPlayer_Ready_Pistol->ShotBullet(false);

	if (_owner.m_nowAnimeFrm >= _owner.m_shotFrmMax)
	{
		// 現在のアニメーションフレームを初期化する
		_owner.m_nowAnimeFrm = 0.0f;
	}
}

void Player::ActionReload_Idle::Enter(Player& _owner)
{
	// プレイヤーの状態が"リロード状態"でないとき
	if (_owner.m_sType != Player::SituationType::Reload)
	{
		// プレイヤーの状態を"リロード状態"にし、
		// 上半身と下半身のアニメーションを"Reload"に切り替え
		_owner.m_sType = Player::SituationType::Reload;
		_owner.ChangeUpperBodyAnimation("Reload", false);
		_owner.ChangeLowerBodyAnimation("Reload", false);
	}
	// "リロード状態"であるとき
	else
	{
		// 現在のアニメーションフレームが0.0fでないとき
		if (_owner.m_nowAnimeFrm != 0.0f)
		{
			// 上半身と下半身のアニメーションを"Reload"に切り替え
			_owner.ChangeUpperBodyAnimation("Reload", false, _owner.m_nowAnimeFrm);
			_owner.ChangeLowerBodyAnimation("Reload", false, _owner.m_nowAnimeFrm);
		}
		// 0.0fであるとき
		else
		{
			// アニメーションを初期化し、
			// 上半身と下半身のアニメーションを"Reload"に切り替え
			_owner.m_nowAnimeFrm = 0.0f;
			_owner.ChangeUpperBodyAnimation("Reload", false);
			_owner.ChangeLowerBodyAnimation("Reload", false);
		}
	}
}

void Player::ActionReload_Idle::Update(Player& _owner)
{
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<Reticle>				spReticle		= _owner.m_wpReticle.lock();
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();
	
	// 移動方向を初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// 現在のアニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// 各移動キーが押されたとき、対応した方向に向ける
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// レティクルを表示し、カメラの状態を切り替え
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
	}
	else
	{
		// レティクルを非表示にし、カメラの状態を切り替え、
		// ActionIdleに切り替え
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
	}

	// 移動方向のベクトルが0より大きいとき
	if (_owner.m_moveDir.LengthSquared() > 0)
	{
		// ActionReload_Runに切り替え
		_owner.ChangeActionState(std::make_shared<ActionReload_Run>());
		return;
	}

	// アニメーションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		// 銃の情報があるとき
		if (spReady_Pistol)
		{
			// 銃をリロード
			spReady_Pistol->Reload();
		}

		// アニメーションフレームを設定し、ActionIdleに切り替え
		_owner.m_nowAnimeFrm = _owner.m_reloadFrmMax;
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
	}

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionReload_Idle::Exit(Player& _owner)
{
	// アニメーションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		// 現在のアニメーションフレームを初期化する
		_owner.m_nowAnimeFrm = 0.0f;
	}
}

void Player::ActionReload_Walk::Enter(Player& _owner)
{
	// プレイヤーの状態が"リロード状態"でないとき
	if (_owner.m_sType != Player::SituationType::Reload)
	{
		// プレイヤーの状態を"リロード状態"にし、
		// 上半身と下半身のアニメーションを"Reload_Walk"に切り替え
		_owner.m_sType = Player::SituationType::Reload;
		_owner.ChangeUpperBodyAnimation("Reload_Walk", false);
		_owner.ChangeLowerBodyAnimation("Reload_Walk", false);
	}
	// "リロード状態"であるとき
	else
	{
		// 現在のアニメーションフレームが0.0fでないとき
		if (_owner.m_nowAnimeFrm != 0.0f)
		{
			// 上半身と下半身のアニメーションを"Reload_Walk"に切り替え
			_owner.ChangeUpperBodyAnimation("Reload_Walk", false, _owner.m_nowAnimeFrm);
			_owner.ChangeLowerBodyAnimation("Reload_Walk", false, _owner.m_nowAnimeFrm);
		}
		// 0.0fであるとき
		else
		{
			// 現在のアニメーションフレームを初期化し、
			// 上半身と下半身のアニメーションを"Reload_Walk"に切り替え
			_owner.m_nowAnimeFrm = 0.0f;
			_owner.ChangeUpperBodyAnimation("Reload_Walk", false);
			_owner.ChangeLowerBodyAnimation("Reload_Walk", false);
		}
	}
}

void Player::ActionReload_Walk::Update(Player& _owner)
{
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();

	// 現在のアニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// 現在の移動速度が歩行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_walkMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_walkMoveSpd;
	}

	// CTRLキーが押されたとき
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		// 移動方向を初期化
		_owner.m_moveDir = Math::Vector3::Zero;

		// 各移動キーが押されたとき、対応した方向に向ける
		if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
		if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
		if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
		if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}
	}
	// キーが離されたとき
	else
	{
		// 移動方向のベクトルの長さが0であるとき
		if (_owner.m_moveDir.LengthSquared() == 0)
		{
			// 現在のアニメーションフレームが最後まで再生されたら
			if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
			{
				// 銃の情報があるとき
				if (spReady_Pistol)
				{
					// 銃をリロード
					spReady_Pistol->Reload();
				}

				// ActionIdleに切り替え
				_owner.ChangeActionState(std::make_shared<ActionIdle>());
				return;
			}
			// 最後まで再生されていなかったら
			else
			{
				//ActionReload_Idleに切り替え
				_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
				return;
			}
		}
		// ベクトルの長さが0でないとき
		else
		{
			// 現在のアニメーションフレームが最後まで再生されたら
			if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
			{
				// 銃の情報があるとき
				if (spReady_Pistol)
				{
					// 銃をリロード
					spReady_Pistol->Reload();
				}

				// ActionRunに切り替え
				_owner.ChangeActionState(std::make_shared<ActionRun>());
				return;
			}
			// 最後まで再生されていなかったら
			else
			{
				//ActionReload_Runに切り替え
				_owner.ChangeActionState(std::make_shared<ActionReload_Run>());
				return;
			}
		}
	}

	// 現在のアニメーションフレームが最後まで再生されたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		// 移動方向のベクトルの長さが0より大きいとき
		if (_owner.m_moveDir.LengthSquared() > 0)
		{
			// 銃の情報があるとき
			if (spReady_Pistol)
			{
				// 銃をリロード
				spReady_Pistol->Reload();
			}

			//ActionWalkに切り替え
			_owner.ChangeActionState(std::make_shared<ActionWalk>());
			return;
		}
	}

	// "C"キーが押されたとき
	if (GetAsyncKeyState('C') & 0x8000)
	{
		// 姿勢状態切り替えフラグがfalseのとき
		if (!_owner.m_posKeyFlg)
		{
			// 姿勢状態切り替えフラグをtrueにし、
			// カメラの状態を切り替え、ActionSitに切り替え
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
	// キーが離されたとき
	else
	{
		// 姿勢状態切り替えフラグをfalseにする
		_owner.m_posKeyFlg = false;
	}

	// カメラ情報があるとき
	if (spCamera)
	{
		// 移動方向の法線ベクトルをカメラの回転行列に変換
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionReload_Walk::Exit(Player& _owner)
{
	// アニメーションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		// 現在のアニメーションフレームを初期化する
		_owner.m_nowAnimeFrm = 0.0f;
	}
}

void Player::ActionReload_Run::Enter(Player& _owner)
{
	// プレイヤーの状態が"リロード状態"でないとき
	if (_owner.m_sType != Player::SituationType::Reload)
	{
		// プレイヤーの状態を"リロード状態"にし、
		// 上半身と下半身のアニメーションを"Reload_Run"に切り替え
		_owner.m_sType = Player::SituationType::Reload;
		_owner.ChangeUpperBodyAnimation("Reload_Run", false);
		_owner.ChangeLowerBodyAnimation("Reload_Run", false);
	}
	// "リロード状態"であるとき
	else
	{
		// 現在のアニメーションフレームが0.0fでないとき
		if (_owner.m_nowAnimeFrm <= _owner.m_reloadFrmMax)
		{
			// 上半身と下半身のアニメーションを"Reload_Run"に切り替え
			_owner.ChangeUpperBodyAnimation("Reload_Run", false, _owner.m_nowAnimeFrm);
			_owner.ChangeLowerBodyAnimation("Reload_Run", false, _owner.m_nowAnimeFrm);
		}
	}
}

void Player::ActionReload_Run::Update(Player& _owner)
{
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();

	// 現在の移動速度が走行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_runMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_runMoveSpd;
	}

	// 移動方向を初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// 各移動キーが押されたとき、対応した方向に向ける
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}

	// 移動方向のベクトルの長さが0であるとき
	if (_owner.m_moveDir.LengthSquared() == 0)
	{
		// ActionReload_Idleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
		return;
	}
	// 0でないとき
	else
	{
		// CTRLキーが押されたとき
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			// ActoinReload_Walkに切り替え
			_owner.ChangeActionState(std::make_shared<ActionReload_Walk>());
			return;
		}
	}

	// 現在のアニメーションフレームが最後まで再生されたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		// 銃の情報があるとき
		if (spReady_Pistol)
		{
			// 銃をリロード
			spReady_Pistol->Reload();
		}

		// ActionIdleに切り替え
		_owner.m_nowAnimeFrm = _owner.m_reloadFrmMax;
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}

	// カメラ情報があるとき
	if (spCamera)
	{
		// 移動方向の法線ベクトルをカメラの回転行列に変換
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionReload_Run::Exit(Player& _owner)
{
	// アニメーションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadFrmMax)
	{
		// 現在のアニメーションフレームを初期化する
		_owner.m_nowAnimeFrm = 0.0f;
	}
}

void Player::ActionSit::Enter(Player& _owner)
{
	// プレイヤーの姿勢状態が"しゃがみ状態"でないとき
	if (_owner.m_posType != PostureType::Sit)
	{
		// 現在のアニメーションフレームを初期化し、
		// 上半身と下半身のアニメーションを"Sit"に切り替え
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Sit", false);
		_owner.ChangeLowerBodyAnimation("Sit", false);
	}
}

void Player::ActionSit::Update(Player& _owner)
{
	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// アニメーションフレームが最後まで再生されたら
	if (_owner.m_nowAnimeFrm >= _owner.m_sitFrmMax)
	{
		// ActionSit_Idleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}
}

void Player::ActionSit::Exit(Player& _owner)
{
	// アニメーションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_sitFrmMax)
	{
		// 現在のアニメーションフレームを初期化する
		_owner.m_nowAnimeFrm = 0.0f;
	}
}

void Player::ActionStand::Enter(Player& _owner)
{
	// プレイヤーの姿勢状態が"立ち状態"でないとき
	if (_owner.m_posType != PostureType::Stand)
	{
		// 現在のアニメーションフレームを初期化し、
		// 上半身と下半身のアニメーションを"Stand"に切り替え
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.ChangeUpperBodyAnimation("Stand");
		_owner.ChangeLowerBodyAnimation("Stand");
	}
}

void Player::ActionStand::Update(Player& _owner)
{
	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// アニメーションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_standFrmMax)
	{
		// ActionIdleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}
}

void Player::ActionStand::Exit(Player& _owner)
{
	// 現在のアニメションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_standFrmMax)
	{
		// 現在のアニメーションフレームを初期化する
		_owner.m_nowAnimeFrm = 0.0f;
	}
}

void Player::ActionSit_Idle::Enter(Player& _owner)
{
	// プレイヤーの状態が"停止状態"でないとき、
	// またはプレイヤーの姿勢状態が"しゃがみ状態"でないとき
	if (_owner.m_sType != SituationType::Idle || _owner.m_posType != PostureType::Sit)
	{
		// 現在のアニメーションフレームを初期化し、
		// プレイヤーの状態を"停止状態"に切り替え、
		// プレイヤーの姿勢状態を"しゃがみ状態"に切り替える
		_owner.m_nowAnimeFrm	= 0.0f;
		_owner.m_sType			= SituationType::Idle;
		_owner.m_posType		= PostureType::Sit;
		// 上半身と下半身のアニメーションを"Sit_Idle"に切り替え
		_owner.ChangeUpperBodyAnimation("Sit_Idle");
		_owner.ChangeLowerBodyAnimation("Sit_Idle");
	}
}

void Player::ActionSit_Idle::Update(Player& _owner)
{
	// カメラ情報取得
	std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();

	// 移動方向を初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// 各移動キーが押されたとき、対応した方向に向ける
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}

	// 移動方向のベクトルの長さが0より大きいとき
	if (_owner.m_moveDir.LengthSquared() > 0)
	{
		// ActionSit_Walkに切り替え
		_owner.ChangeActionState(std::make_shared<ActionSit_Walk>());
		return;
	}

	// "C"キーが押されたとき
	if (GetAsyncKeyState('C') & 0x8000)
	{
		// 姿勢状態切り替えフラグがfalseのとき
		if (!_owner.m_posKeyFlg)
		{
			// 姿勢状態切り替えフラグをtrueにし、
			// カメラの状態を切り替え、ActionStandに切り替える
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
	// キーが離されたとき
	else
	{
		// 姿勢状態切り替えフラグをfalseにする
		_owner.m_posKeyFlg = false;
	}

	// 右クリックされたとき
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// ActionSit_Readyに切り替え
		_owner.ChangeActionState(std::make_shared<ActionSit_Ready>());
		return;
	}

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionSit_Idle::Exit(Player& _owner)
{
	// 現在のアニメーションフレームを初期化する
	_owner.m_nowAnimeFrm = 0.0f;
}

void Player::ActionSit_Walk::Enter(Player& _owner)
{
	// プレイヤーの状態が"歩行状態"でないとき
	if (_owner.m_sType != SituationType::Walk)
	{
		// 現在のアニメーションフレームを初期化し、
		// プレイヤーの状態を"歩行状態"に切り替え
		_owner.m_nowAnimeFrm = 0.0f;
		_owner.m_sType = SituationType::Walk;
		// 上半身と下半身のアニメーションを"Sit_Walk"に切り替え
		_owner.ChangeUpperBodyAnimation("Sit_Walk");
		_owner.ChangeLowerBodyAnimation("Sit_Walk");
	}
}

void Player::ActionSit_Walk::Update(Player& _owner)
{
	// カメラ情報取得
	const std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();

	// 現在の移動速度がしゃがみ歩行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_sitWalkMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_sitWalkMoveSpd;
	}

	// 移動方向を初期化
	_owner.m_moveDir = Math::Vector3::Zero;

	// 各移動キーが押されたとき、対応した方向に向ける
	if (GetAsyncKeyState('W') & 0x8000) { _owner.m_moveDir += Math::Vector3::Backward;	}
	if (GetAsyncKeyState('S') & 0x8000) { _owner.m_moveDir += Math::Vector3::Forward;	}
	if (GetAsyncKeyState('A') & 0x8000) { _owner.m_moveDir += Math::Vector3::Left;		}
	if (GetAsyncKeyState('D') & 0x8000) { _owner.m_moveDir += Math::Vector3::Right;		}

	// 移動方向のベクトルの長さが0であるとき
	if (_owner.m_moveDir.LengthSquared() == 0)
	{
		// ActionSit_Idleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}

	// "C"キーが押されたとき
	if (GetAsyncKeyState('C') & 0x8000)
	{
		// 姿勢状態切り替えフラグがfalseのとき
		if (!_owner.m_posKeyFlg)
		{
			// 姿勢状態切り替えフラグをtrueにし、
			// カメラの状態を切り替え、ActionStandに切り替え
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
	// キーが離されたとき
	else
	{
		// 姿勢状態切り替えフラグをfalseにする
		_owner.m_posKeyFlg = false;
	}

	// 右クリックされたとき
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// ActionSit_Readyに切り替え
		_owner.ChangeActionState(std::make_shared<ActionSit_Ready>());
	}

	// カメラ情報があるとき
	if (spCamera)
	{
		// 移動方向の法線ベクトルをカメラの回転行列に変換
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

	// 移動方向を初期化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	// 行列作成
	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
}

void Player::ActionSit_Walk::Exit(Player& _owner)
{
	// 現在のアニメーションフレームを初期化する
	_owner.m_nowAnimeFrm = 0.0f;
}

void Player::ActionSit_Ready::Enter(Player& _owner)
{
	// プレイヤーの状態が"構え状態"でないとき、
	// またはプレイヤーの姿勢状態が"しゃがみ状態"でないとき
	if (_owner.m_sType != SituationType::Ready || _owner.m_posType != PostureType::Sit)
	{
		// 現在のアニメーションフレームを初期化し、
		// プレイヤーの状態を"構え状態"に切り替え、
		// プレイヤーの姿勢状態を"しゃがみ状態"に切り替える
		_owner.m_nowAnimeFrm	= 0.0f;
		_owner.m_sType			= SituationType::Ready;
		_owner.m_posType		= PostureType::Sit;
		// 上半身と下半身のアニメーションを"Sit_Ready"に切り替え
		_owner.ChangeUpperBodyAnimation("Sit_Ready");
		_owner.ChangeLowerBodyAnimation("Sit_Ready");
	}
}

void Player::ActionSit_Ready::Update(Player& _owner)
{
	// カメラとレティクル情報取得
	const std::shared_ptr<TPSCamera> spCamera = _owner.m_wpCamera.lock();
	const std::shared_ptr<Reticle>	spReticle = _owner.m_wpReticle.lock();

	// カメラの行列を格納するためにローカル行列を宣言
	Math::Matrix camRotMat;

	// カメラの情報があれば、カメラの行列をローカル行列に格納
	if (spCamera)
	{
		camRotMat = spCamera->GetRotationMatrix();
	}

	// 右クリックされたとき
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// レティクルを表示する
		spReticle->SetActive(true);

		// カメラの状態を切り替え
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
	// クリックが離されたとき
	else
	{
		// レティクルを非表示にする
		spReticle->SetActive(false);

		// カメラの状態を切り替え
		switch (spCamera->GetPastCamType())
		{
		case TPSCamera::CameraType::SitR:
			spCamera->ChangeSitR();
			break;
		case TPSCamera::CameraType::SitL:
			spCamera->ChangeSitL();
			break;
		}

		// ActionSit_Idleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionSit_Idle>());
		return;
	}

	// カメラのローカル行列を格納
	_owner.m_mRot = camRotMat;
}

void Player::ActionSit_Ready::Exit(Player& _owner)
{
	// 現在のアニメーションフレームを初期化する
	_owner.m_nowAnimeFrm = 0.0f;
}
//================================================================================================================================
// ステートパターン管理系
//================================================================================================================================
