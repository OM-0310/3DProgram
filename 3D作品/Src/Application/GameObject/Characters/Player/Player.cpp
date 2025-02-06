#include "Player.h"

#include "Player_Main/Player_Main.h"
#include "Player_Disarm/Player_Disarm.h"
#include "Player_Disarm_Pistol/Player_Disarm_Pistol.h"
#include "Player_Ready_Pistol/Player_Ready_Pistol.h"

#include "../Enemy/Enemy_1/Enemy_1.h"
#include "../Enemy/Enemy_2/Enemy_2.h"
#include "../Enemy/Enemy_3/Enemy_3.h"

#include "../../Camera/TPSCamera/TPSCamera.h"

#include "../../Gimmicks/LockedDoor/LockedDoor.h"

#include "../../Item/CardKey/CardKey.h"
#include "../../Item/SecretFile/SecretFile.h"

#include "../../Goal/Goal.h"

#include "../../UI/Reticle/Reticle.h"
#include "../../UI/CollectCardKeyUI/CollectCardKeyUI.h"
#include "../../UI/CollectSecretFileUI/CollectSecretFileUI.h"
#include "../../UI/RestraintUI/RestraintUI.h"
#include "../../UI/CollectUIBack/CollectUIBack.h"
#include "../../UI/MiniMapUI/MiniMapUI.h"
#include "../../UI/MiniMapUIBack/MiniMapUIBack.h"
#include "../../UI/MainMissionUI/MainMissionUI.h"
#include "../../UI/SubMissionUI/SubMissionUI.h"
#include "../../UI/CurrentLocation/CurrentLocation.h"
#include "../../UI/CardKeyLocation/CardKeyLocation.h"
#include "../../UI/SecretFileLocation/SecretFileLocation.h"
#include "../../UI/KillUI/KillUI.h"
#include "../../UI/InterrogationUI/InterrogationUI.h"
#include "../../UI/AnswerUI/AnswerUI.h"
#include "../../UI/NoAnswerUI/NoAnswerUI.h"
#include "../../UI/AnswerUIBack/AnswerUIBack.h"
#include "../../UI/MapUpdateUI/MapUpdateUI.h"
#include "../../UI/MapUpdateUIBack/MapUpdateUIBack.h"

#include "../../Bullet/Bullet.h"

#include "../../../Scene/SceneManager.h"
#include "../../../main.h"

void Player::Init()
{
	m_spAnimator	= std::make_shared<KdAnimator>();
	m_nowAnimeFrm	= 0.0f;
	m_animeSpeed	= m_normalAnimSpd;

	ChangeActionState(std::make_shared<ActionIdle>());

	m_spOpenMapSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/Game/OpenMiniMap.wav");
	m_spOpenMapSound->SetVolume(m_openMapVol);

	m_spCloseMapSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/Game/CloseMiniMap.wav");
	m_spCloseMapSound->SetVolume(m_closeMapVol);

	m_spReadySound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Pistol_Ready.wav");
	m_spReadySound->SetVolume(m_readyVol);

	m_spShotSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Pistol_Shot.wav");
	m_spShotSound->SetVolume(m_shotVol);

	m_spReloadSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Reload.wav");
	m_spReloadSound->SetVolume(m_reloadVol);

	m_spRestraintSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Restraint.wav");
	m_spRestraintSound->SetVolume(m_restraintVol);

	m_spRunSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Run.wav");
	m_spRunSound->SetVolume(m_runVol);

	m_spWalkSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Walk.wav");
	m_spWalkSound->SetVolume(m_walkVol);

	m_spItemCollectSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/Game/ItemCollect.wav");
	m_spItemCollectSound->SetVolume(m_itemCollVol);

	m_spExeSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Execution.wav");
	m_spExeSound->SetVolume(m_exeVol);

	m_spDeathSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Death.wav");
	m_spDeathSound->SetVolume(m_deathVol);

	m_pos			= { 0.0f,-0.9f,-50.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.0f;

	for (uint16_t i = 0; i < m_bitsKeyFlg.size(); ++i)
	{
		m_bitsKeyFlg.reset(i);
	}

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}

	m_diffVec		= Math::Vector3::Zero;

	m_sType			= SituationType::Idle;
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

	// 死亡処理
	DeathProc();

	// 各種「状態」に応じた更新処理を実行する
	if (m_nowState)
	{
		m_nowState->Update(*this);
	}

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;

	Application::Instance().m_log.Clear();
	Application::Instance().m_log.AddLog("m_pos={ %.2f, %.2f, %.2f }", m_pos.x, m_pos.y, m_pos.z);

	//m_pDebugWire->AddDebugSphere(readySphereInfo.m_sphere.Center, readySphereInfo.m_sphere.Radius, kBlueColor);
}

void Player::PostUpdate()
{
	m_spReadySound->SetPos(GetPos());
	m_spShotSound->SetPos(GetPos());
	m_spReloadSound->SetPos(GetPos());
	m_spRestraintSound->SetPos(GetPos());
	m_spRunSound->SetPos(GetPos());
	m_spWalkSound->SetPos(GetPos());
	m_spExeSound->SetPos(GetPos());
	m_spDeathSound->SetPos(GetPos());
	
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
	const std::shared_ptr<MiniMapUI>			spMiniMapUI			= m_wpMiniMapUI.lock();
	const std::shared_ptr<MiniMapUIBack>		spMiniMapUIBack		= m_wpMiniMapUIBack.lock();
	const std::shared_ptr<MainMissionUI>		spMainMissionUI		= m_wpMainMissionUI.lock();
	const std::shared_ptr<SubMissionUI>			spSubMissionUI		= m_wpSubMissionUI.lock();
	const std::shared_ptr<CurrentLocation>		spCurrentLocation	= m_wpCurrentLocation.lock();
	const std::shared_ptr<CardKeyLocation>		spCardKeyLocation	= m_wpCardKeyLocation.lock();
	const std::shared_ptr<SecretFileLocation>	spSecretFileLocation = m_wpSecretFileLocation.lock();

	if (GetAsyncKeyState(VK_TAB) & 0x8000)
	{
		if (!m_bitsKeyFlg.test(KeyFlgType::OpenMapKey))
		{
			if (!m_bitsEachFlg.test(EachFlgType::MiniMapFlg))
			{
				m_bitsEachFlg.set(EachFlgType::MiniMapFlg, true);

				if (m_spOpenMapSound->IsStopped())
				{
					m_spOpenMapSound->Play();
				}

				if (m_spCloseMapSound->IsPlaying())
				{
					m_spCloseMapSound->Stop();
				}
			}
			else
			{
				m_bitsEachFlg.set(EachFlgType::MiniMapFlg, false);

				if (m_spCloseMapSound->IsStopped())
				{
					m_spCloseMapSound->Play();
				}

				if (m_spOpenMapSound->IsPlaying())
				{
					m_spOpenMapSound->Stop();
				}
			}

			m_bitsKeyFlg.set(KeyFlgType::OpenMapKey, true);

			// ミニマップUIの情報があるとき
			if (spMiniMapUI)
			{
				// ミニマップの表示フラグがfalseのとき
				if (!spMiniMapUI->GetActive())
				{
					// 表示フラグをtrueにする
					spMiniMapUI->Open(true);
				}
				// trueのとき
				else
				{
					// 表示フラグをfalseにする
					spMiniMapUI->Open(false);
				}
			}

			// ミニマップ背景UIの情報があるとき
			if (spMiniMapUIBack)
			{
				// ミニマップ背景の表示フラグがfalseのとき
				if (!spMiniMapUIBack->GetActive())
				{
					// 表示フラグをtrueにする
					spMiniMapUIBack->Open(true);
				}
				// trueのとき
				else
				{
					// 表示フラグをfalseにする
					spMiniMapUIBack->Open(false);
				}
			}

			// メインミッションUI情報があるとき
			if (spMainMissionUI)
			{
				// メインミッションUIの表示フラグがfalseのとき
				if (!spMainMissionUI->GetActive())
				{
					// 表示フラグをtrueにする
					spMainMissionUI->Open(true);
				}
				// trueのとき
				else
				{
					// 表示フラグをfalseにする
					spMainMissionUI->Open(false);
				}
			}

			// サブミッションUI情報があるとき
			if (spSubMissionUI)
			{
				// サブミッションUIの表示フラグがfalseのとき
				if (!spSubMissionUI->GetActive())
				{
					// 表示フラグをtrueにする
					spSubMissionUI->Open(true);
				}
				// trueのとき
				else
				{
					// 表示フラグをfalseにする
					spSubMissionUI->Open(false);
				}
			}

			// 現在地UIの情報があるとき
			if (spCurrentLocation)
			{
				// 現在地UIの表示フラグがfalseのとき
				if (!spCurrentLocation->GetActive())
				{
					// 表示フラグをtrueにする
					spCurrentLocation->Open(true);
				}
				// falseのとき
				else
				{
					// 表示フラグをfalseにする
					spCurrentLocation->Open(false);
				}
			}

			// カードキー位置UIの情報があるとき
			if (spCardKeyLocation)
			{
				// カードキー位置表示許可フラグがtrueのとき
				if (spCardKeyLocation->GetPermissionFlg())
				{
					// 表示フラグがfalseのとき
					if (!spCardKeyLocation->GetActive())
					{
						// 表示フラグをtrueにする
						spCardKeyLocation->Open(true);
					}
					// trueのとき
					else
					{
						// 表示フラグをfalseにする
						spCardKeyLocation->Open(false);
					}
				}
			}

			// 機密ファイル位置UIの情報があるとき
			if (spSecretFileLocation)
			{
				// 機密ファイル位置表示許可フラグがtrueのとき
				if (spSecretFileLocation->GetPermissionFlg())
				{
					// 表示フラグがfalseのとき
					if (!spSecretFileLocation->GetActive())
					{
						// 表示フラグをtrueにする
						spSecretFileLocation->Open(true);
					}
					// trueのとき
					else
					{
						// 表示フラグをfalseにする
						spSecretFileLocation->Open(false);
					}
				}
			}
		}
	}
	else
	{
		m_bitsKeyFlg.set(KeyFlgType::OpenMapKey, false);
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
		if (!m_bitsKeyFlg.test(KeyFlgType::CollectKey))
		{
			// キーフラグをtrueにする
			m_bitsKeyFlg.set(KeyFlgType::CollectKey, true);

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

					m_spItemCollectSound->Play();
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

					m_spItemCollectSound->Play();
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

					m_spItemCollectSound->Play();

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
		m_bitsKeyFlg.set(KeyFlgType::CollectKey, false);
	}
}

//================================================================================================================================
// 自動リロード処理
//================================================================================================================================
void Player::AutoReloadProc()
{
	const std::shared_ptr<Player_Ready_Pistol> spPlayer_Ready_Pistol = m_wpPlayer_Ready_Pistol.lock();
	short nowBullet = 0;

	// 銃の情報があるとき
	if (spPlayer_Ready_Pistol)
	{
		// 残弾数を取得
		nowBullet = spPlayer_Ready_Pistol->GetNowBullet();

		// 残弾数が空のとき
		if (nowBullet <= spPlayer_Ready_Pistol->GetMagazinEmpty())
		{
			// Reload_Idleに切り替え
			ChangeActionState(std::make_shared<ActionReload_Idle>());
			return;
		}
	}
}

//================================================================================================================================
// アニメーション切り替え処理
//================================================================================================================================
void Player::ChangeAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	const std::shared_ptr<Player_Main>			spPlayer_Main = m_wpPlayer_Main.lock();
	const std::shared_ptr<Player_Ready_Pistol>	spPlayer_Ready_Pistol = m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<Player_Disarm>		spPlayer_Disarm = m_wpPlayer_Disarm.lock();
	const std::shared_ptr<Player_Disarm_Pistol>	spPlayer_Disarm_Pistol = m_wpPlayer_Disarm_Pistol.lock();
	if (spPlayer_Main)
	{
		spPlayer_Main->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spPlayer_Ready_Pistol)
	{
		spPlayer_Ready_Pistol->ChangeAnimation(_animeName, _isLoop, _time);
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
			if (!m_bitsKeyFlg.test(KeyFlgType::ChangeKey))
			{
				m_bitsKeyFlg.set(KeyFlgType::ChangeKey, true);
				spCamera->ChangeTPSL();
			}
			break;
		case TPSCamera::CameraType::TpsL:
			if (!m_bitsKeyFlg.test(KeyFlgType::ChangeKey))
			{
				m_bitsKeyFlg.set(KeyFlgType::ChangeKey, true);
				spCamera->ChangeTPSR();
			}
			break;
		case TPSCamera::CameraType::AimR:
			if (!m_bitsKeyFlg.test(KeyFlgType::ChangeKey))
			{
				m_bitsKeyFlg.set(KeyFlgType::ChangeKey, true);
				spCamera->ChangeAimL();
			}
			break;
		case TPSCamera::CameraType::AimL:
			if (!m_bitsKeyFlg.test(KeyFlgType::ChangeKey))
			{
				m_bitsKeyFlg.set(KeyFlgType::ChangeKey, true);
				spCamera->ChangeAimR();
			}
			break;
		}
	}
	// キーが離されたとき
	else
	{
		// 視点切り替えキーフラグをfalseにする
		m_bitsKeyFlg.set(KeyFlgType::ChangeKey, false);
	}
}

void Player::RestraintProc()
{
	const std::shared_ptr<Enemy_1>		spEnemy_1 = m_wpEnemy_1.lock();
	const std::shared_ptr<Enemy_2>		spEnemy_2 = m_wpEnemy_2.lock();
	const std::shared_ptr<Enemy_3>		spEnemy_3 = m_wpEnemy_3.lock();
	const std::shared_ptr<RestraintUI>	spRestraintUI = m_wpRestraintUI.lock();

	Math::Vector3 parentPos1, parentPos2, parentPos3;
	Math::Vector3 diffVec1, diffVec2, diffVec3;

	if (spEnemy_1)
	{
		parentPos1 = spEnemy_1->GetPos();

		// プレイヤーと敵の座標の差ベクトルを算出
		diffVec1 = parentPos1 - m_pos;

		if (spEnemy_1->GetSitType() != Enemy_1::SituationType::Aressted_Death &&
			spEnemy_1->GetSitType() != Enemy_1::SituationType::Death)
		{
			// 差ベクトルが拘束エリア範囲内のとき
			if (diffVec1.Length() <= m_tightArea)
			{
				// 左クリックしたとき
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					m_bitsEachFlg.set(EachFlgType::RestEnemy1Flg, true);

					// 拘束UI情報があるとき
					if (spRestraintUI)
					{
						spRestraintUI->SetRestraintFlg(true);
					}

					// "ActionRestraint"に切り替え
					ChangeActionState(std::make_shared<ActionRestraint>());
					return;
				}
			}
		}

	}

	if (spEnemy_2)
	{
		parentPos2 = spEnemy_2->GetPos();

		// プレイヤーと敵の座標の差ベクトルを算出
		diffVec2 = parentPos2 - m_pos;

		if (spEnemy_2->GetSitType() != Enemy_2::SituationType::Aressted_Death &&
			spEnemy_2->GetSitType() != Enemy_2::SituationType::Death)
		{
			// 差ベクトルが拘束エリア範囲内のとき
			if (diffVec2.Length() <= m_tightArea)
			{
				// 左クリックしたとき
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					m_bitsEachFlg.set(EachFlgType::RestEnemy2Flg, true);

					// 拘束UI情報があるとき
					if (spRestraintUI)
					{
						spRestraintUI->SetRestraintFlg(true);
					}

					// "ActionRestraint"に切り替え
					ChangeActionState(std::make_shared<ActionRestraint>());
					return;
				}

			}
		}
	}

	if (spEnemy_3)
	{
		parentPos3 = spEnemy_3->GetPos();

		// プレイヤーと敵の座標の差ベクトルを算出
		diffVec3 = parentPos3 - m_pos;

		if (spEnemy_3->GetSitType() != Enemy_3::SituationType::Aressted_Death &&
			spEnemy_3->GetSitType() != Enemy_3::SituationType::Death)
		{
		// 差ベクトルが拘束エリア範囲内のとき
			if (diffVec3.Length() <= m_tightArea)
			{
				// 左クリックしたとき
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				{
					m_bitsEachFlg.set(EachFlgType::RestEnemy3Flg, true);

					// 拘束UI情報があるとき
					if (spRestraintUI)
					{
						spRestraintUI->SetRestraintFlg(true);
					}

					// "ActionRestraint"に切り替え
					ChangeActionState(std::make_shared<ActionRestraint>());
					return;
				}
			}
		}
	}
}

void Player::DeathProc()
{
	const std::shared_ptr<Player_Main> spMain = m_wpPlayer_Main.lock();

	if (spMain)
	{
		if (spMain->GetHP() <= 0)
		{
			ChangeActionState(std::make_shared<ActionDeath>());
			return;
		}
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
		// プレイヤーの状態が"停止状態"でないとき
		if (_owner.m_sType != SituationType::Idle)
		{
			// プレイヤーの状態を"停止状態"にする
			_owner.m_sType = SituationType::Idle;
			// プレイヤーの上半身と下半身のアニメーションを"Idle"に切り替え
			_owner.ChangeAnimation("Idle", _owner.m_nowAnimeFrm);
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;
		}
	}
	else
	{
		// 現在のアニメーションフレームを初期化
		_owner.m_nowAnimeFrm = 0.0f;
		// プレイヤーの状態が"停止状態"でないとき
		if (_owner.m_sType != SituationType::Idle)
		{
			// プレイヤーの状態を"停止状態"にする
			_owner.m_sType = SituationType::Idle;
			// プレイヤーの上半身と下半身のアニメーションを"Idle"に切り替え
			_owner.ChangeAnimation("Idle");
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;
		}
	}
}

void Player::ActionIdle::Update(Player& _owner)
{
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<RestraintUI>			spRestraintUI	= _owner.m_wpRestraintUI.lock();

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

	// 拘束処理
	_owner.RestraintProc();

	// 自動リロード処理
	_owner.AutoReloadProc();

	// 銃の情報があるとき
	if (spReady_Pistol)
	{
		// "R"キーを押されたとき
		if (GetAsyncKeyState('R') & 0x8000)
		{
			// 残弾数がマガジンサイズ以下のとき
			if (spReady_Pistol->GetNowBullet() < spReady_Pistol->GetMagazinSize())
			{
				// リロードキーフラグがfalseのとき
				if (!_owner.m_bitsKeyFlg.test(KeyFlgType::ReloadKey))
				{
					// リロードフラグをtrueにし、ActionReload_Idleに切り替え
					_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, true);
					_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
				}
			}
		}
		// キーが離されたとき
		else
		{
			_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, false);
		}
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
		// アニメーションを"Walk"に切り替え
		_owner.ChangeAnimation("Walk");
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;
		_owner.m_spWalkSound->Play(true);
	}
}

void Player::ActionWalk::Update(Player& _owner)
{
	const std::shared_ptr<RestraintUI>			spRestraintUI	= _owner.m_wpRestraintUI.lock();
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

	// 拘束処理
	_owner.RestraintProc();

	// 自動リロード処理
	_owner.AutoReloadProc();

	// 銃の情報があるとき
	if (spReady_Pistol)
	{
		// "Rキーが押されたとき"
		if (GetAsyncKeyState('R') & 0x8000)
		{
			// 残弾数がマガジンサイズ以下のとき
			if (spReady_Pistol->GetNowBullet() < spReady_Pistol->GetMagazinSize())
			{
				// リロードキーフラグがfalseのとき
				if (!_owner.m_bitsKeyFlg.test(KeyFlgType::ReloadKey))
				{
					// リロードキーフラグをtrueにし、ActionReload_Walkに切り替え
					_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, true);
					_owner.ChangeActionState(std::make_shared<ActionReload_Walk>());
					return;
				}
			}
		}
		// キーフラグが離されたとき
		else
		{
			// リロードキーフラグをfalseにする
			_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, false);
		}
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

	_owner.m_spWalkSound->Stop();
}

void Player::ActionRun::Enter(Player& _owner)
{
	// プレイヤーの状態が"走行状態"でないとき
	if (_owner.m_sType != SituationType::Run)
	{
		// プレイヤーの状態を"走行状態"に切り替えて、
		// アニメーションを"Run"に切り替え
		_owner.m_sType = SituationType::Run;
		_owner.ChangeAnimation("Run");

		_owner.m_animeSpeed = _owner.m_runAnimSpd;
		_owner.m_spRunSound->Play(true);
	}
}

void Player::ActionRun::Update(Player& _owner)
{
	const std::shared_ptr<RestraintUI>			spRestraintUI	= _owner.m_wpRestraintUI.lock();
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

	// 拘束処理
	_owner.RestraintProc();

	// 自動リロード処理
	_owner.AutoReloadProc();

	// CTRLキーが押されたとき
	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		_owner.ChangeActionState(std::make_shared<ActionWalk>());
		return;
	}

	// 銃の情報があるとき
	if (spReady_Pistol)
	{
		// "R"キーが押されたとき
		if (GetAsyncKeyState('R') & 0x8000)
		{
			// 残弾数がマガジンサイズ以下のとき
			if (spReady_Pistol->GetNowBullet() < spReady_Pistol->GetMagazinSize())
			{
				// リロードキーフラグがfalseのとき
				if (!_owner.m_bitsKeyFlg.test(KeyFlgType::ReloadKey))
				{
					// リロードキーフラグをtrueにし、ActionReload_Runに切り替え
					_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, true);
					_owner.ChangeActionState(std::make_shared<ActionReload_Run>());
				}
			}
		}
		// 離されたとき
		else
		{
			// リロードキーフラグをfalseにする
			_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, false);
		}
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

	_owner.m_spRunSound->Stop();
}

void Player::ActionReady::Enter(Player& _owner)
{
	// プレイヤーの状態が"構え状態"でないとき
	if (_owner.m_sType != SituationType::Ready)
	{
		// プレイヤーの状態を"構え状態"に切り替えて、
		// アニメーションを"Ready"に切り替え
		_owner.m_sType = SituationType::Ready;
		_owner.ChangeAnimation("Ready");
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;

		_owner.m_spReadySound->Play();
	}
}

void Player::ActionReady::Update(Player& _owner)
{
	// カメラ情報とレティクル情報取得
	const std::shared_ptr<TPSCamera>			spCamera		= _owner.m_wpCamera.lock();
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<Reticle>				spReticle		= _owner.m_wpReticle.lock();

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
	if (_owner.m_moveDir.LengthSquared() != 0)
	{
		if (_owner.m_spRunSound->IsStopped())
		{
			_owner.m_spRunSound->Play(true);
		}
	}
	else
	{
		_owner.m_spRunSound->Stop();
	}

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
			if (!_owner.m_bitsKeyFlg.test(KeyFlgType::ShotKey))
			{
				// 発射キーフラグをtrueにし、ActionShotに切り替え
				_owner.m_bitsKeyFlg.set(KeyFlgType::ShotKey, true);
				_owner.ChangeActionState(std::make_shared<ActionShot>());
			}
		}
		// クリックが離されたとき
		else
		{
			// 発射キーフラグをfalseにする
			_owner.m_bitsKeyFlg.set(KeyFlgType::ShotKey, false);
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

	// 銃の情報があるとき
	if (spReady_Pistol)
	{
		// "R"キーを押されたとき
		if (GetAsyncKeyState('R') & 0x8000)
		{
			// 残弾数がマガジンサイズ以下のとき
			if (spReady_Pistol->GetNowBullet() < spReady_Pistol->GetMagazinSize())
			{
				// リロードキーフラグがfalseのとき
				if (!_owner.m_bitsKeyFlg.test(KeyFlgType::ReloadKey))
				{
					// リロードフラグをtrueにし、ActionReload_Idleに切り替え
					_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, true);
					_owner.ChangeActionState(std::make_shared<ActionReload_Idle>());
				}
			}
		}
		// キーが離されたとき
		else
		{
			_owner.m_bitsKeyFlg.set(KeyFlgType::ReloadKey, false);
		}
	}

	// カメラの情報があるとき
	if (spCamera)
	{
		// ベクトルの向きをY軸の回転行列で変換
		_owner.m_moveDir = _owner.m_moveDir.TransformNormal(_owner.m_moveDir, spCamera->GetRotationYMatrix());
	}

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
		// アニメーションを"Shot"に切り替え
		_owner.m_sType = SituationType::Shot;
		_owner.ChangeAnimation("Shot", false);
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;

		_owner.m_spShotSound->Play();
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

void Player::ActionRestraint::Enter(Player& _owner)
{
	// プレイヤーの状態が"リロード状態"でないとき
	if (_owner.m_sType != Player::SituationType::Restraint)
	{
		// プレイヤーの状態を"リロード状態"にし、
		// アニメーションを"Reload"に切り替え
		_owner.m_sType = Player::SituationType::Restraint;
		_owner.ChangeAnimation("Restraint", false);
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;

		_owner.m_spRestraintSound->Play();
	}
}

void Player::ActionRestraint::Update(Player& _owner)
{
	const std::shared_ptr<Enemy_1> spEnemy_1 = _owner.m_wpEnemy_1.lock();
	const std::shared_ptr<Enemy_2> spEnemy_2 = _owner.m_wpEnemy_2.lock();
	const std::shared_ptr<Enemy_3> spEnemy_3 = _owner.m_wpEnemy_3.lock();

	// 現在の移動速度が停止速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_zeroMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_zeroMoveSpd;
	}

	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// アニメーションフレームが掴み時のフレームのとき
	if (_owner.m_nowAnimeFrm >= _owner.m_grabFrm)
	{

		if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy1Flg))
		{
			if (spEnemy_1)
			{
				spEnemy_1->ChangeAressted();
			}
		}

		if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy2Flg))
		{
			if (spEnemy_2)
			{
				spEnemy_2->ChangeAressted();
			}
		}

		if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy3Flg))
		{
			if (spEnemy_3)
			{
				spEnemy_3->ChangeAressted();
			}
		}
	}

	// アニメーションフレームが終了していたら
	if (_owner.m_nowAnimeFrm >= _owner.m_restFrmMax)
	{
		// ActionRestraint_Idleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionRestraint_Idle>());
		return;
	}
}

void Player::ActionRestraint::Exit(Player& _owner)
{
	// アニメーションフレームを初期化
	_owner.m_nowAnimeFrm = 0.0f;
}

void Player::ActionRestraint_Idle::Enter(Player& _owner)
{
	// プレイヤーの状態が"リロード状態"でないとき
	if (_owner.m_sType != Player::SituationType::Restraint_Idle)
	{
		// プレイヤーの状態を"リロード状態"にし、
		// アニメーションを"Restraint_Idle"に切り替え
		_owner.m_sType = Player::SituationType::Restraint_Idle;
		_owner.ChangeAnimation("Restraint_Idle");
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;
	}
}

void Player::ActionRestraint_Idle::Update(Player& _owner)
{
	const std::shared_ptr<Enemy_1>				spEnemy_1			= _owner.m_wpEnemy_1.lock();
	const std::shared_ptr<Enemy_2>				spEnemy_2			= _owner.m_wpEnemy_2.lock();
	const std::shared_ptr<Enemy_3>				spEnemy_3			= _owner.m_wpEnemy_3.lock();
	const std::shared_ptr<RestraintUI>			spRestraintUI		= _owner.m_wpRestraintUI.lock();
	const std::shared_ptr<KillUI>				spKillUI			= _owner.m_wpKillUI.lock();
	const std::shared_ptr<InterrogationUI>		spInterrogationUI	= _owner.m_wpInterrogationUI.lock();
	const std::shared_ptr<CardKeyLocation>		spCardLocation		= _owner.m_wpCardKeyLocation.lock();
	const std::shared_ptr<SecretFileLocation>	spFileLocation		= _owner.m_wpSecretFileLocation.lock();


	// 左クリックされたとき
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		// 尋問UIの情報があるとき
		if (spInterrogationUI)
		{
			if (!spInterrogationUI->GetUseFlg())
			{
				// Eキーを押されたとき
				if (GetAsyncKeyState('E') & 0x8000)
				{
					// 殺害キーフラグがfalseのとき
					if (!_owner.m_bitsKeyFlg.test(KeyFlgType::KillKey))
					{
						// 殺害キーフラグをtrueにする
						_owner.m_bitsKeyFlg.set(KeyFlgType::KillKey, true);

						// 殺害UIの情報があるとき
						if (spKillUI)
						{
							spKillUI->SetUseFlg(true);

							_owner.m_spItemCollectSound->Play();
						}

						if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy1Flg))
						{
							if (spEnemy_1)
							{
								spEnemy_1->Execution();
							}
						}

						if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy2Flg))
						{
							if (spEnemy_2)
							{
								spEnemy_2->Execution();
							}
						}

						if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy3Flg))
						{
							if (spEnemy_3)
							{
								spEnemy_3->Execution();
							}
						}

						// ActionKillに切り替え
						_owner.ChangeActionState(std::make_shared<ActionKill>());
						return;
					}
				}
				// 離されたとき
				else
				{
					// 殺害キーフラグをfalseにする
					_owner.m_bitsKeyFlg.set(KeyFlgType::KillKey, false);
				}
			}
		}

		// 殺害UIの情報があるとき
		if (spKillUI)
		{
			if (!spKillUI->GetUseFlg())
			{
				// Qキーが押されたとき
				if (GetAsyncKeyState('Q') & 0x8000)
				{
					// 尋問キーフラグがfalseのとき
					if (!_owner.m_bitsKeyFlg.test(KeyFlgType::InterKey))
					{
						// 尋問キーフラグをtrueにする
						_owner.m_bitsKeyFlg.set(KeyFlgType::InterKey, true);

						// 尋問UIの情報があるとき
						if (spInterrogationUI)
						{
							spInterrogationUI->SetUseFlg(true);

							_owner.m_spItemCollectSound->Play();
						}

						if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy1Flg))
						{
							if (spEnemy_1)
							{
								if (!spEnemy_1->GetAnswerFlg())
								{
									spEnemy_1->Answer();

									std::shared_ptr<AnswerUIBack>	answerUIBack;
									answerUIBack = std::make_shared<AnswerUIBack>();
									answerUIBack->Init();
									SceneManager::Instance().AddObject(answerUIBack);

									std::shared_ptr<AnswerUI>		answerUI;
									answerUI = std::make_shared<AnswerUI>();
									answerUI->Init();
									answerUI->SetCardKeyLocation(spCardLocation);
									answerUI->SetSecretFileLocation(spFileLocation);
									SceneManager::Instance().AddObject(answerUI);

									std::shared_ptr<MapUpdateUIBack>mapUpdUIback;
									mapUpdUIback = std::make_shared<MapUpdateUIBack>();
									mapUpdUIback->Init();
									SceneManager::Instance().AddObject(mapUpdUIback);

									std::shared_ptr<MapUpdateUI>	mapUpdUI;
									mapUpdUI = std::make_shared<MapUpdateUI>();
									mapUpdUI->Init();
									SceneManager::Instance().AddObject(mapUpdUI);
								}
								else
								{
									std::shared_ptr<AnswerUIBack>	answerUIBack;
									answerUIBack = std::make_shared<AnswerUIBack>();
									answerUIBack->Init();
									SceneManager::Instance().AddObject(answerUIBack);

									std::shared_ptr<NoAnswerUI>		noAnswerUI;
									noAnswerUI = std::make_shared<NoAnswerUI>();
									noAnswerUI->Init();
									SceneManager::Instance().AddObject(noAnswerUI);
								}
							}
						}

						if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy2Flg))
						{
							if (spEnemy_2)
							{
								if (!spEnemy_2->GetAnswerFlg())
								{
									spEnemy_2->Answer();

									std::shared_ptr<AnswerUIBack>	answerUIBack;
									answerUIBack = std::make_shared<AnswerUIBack>();
									answerUIBack->Init();
									SceneManager::Instance().AddObject(answerUIBack);

									std::shared_ptr<AnswerUI>		answerUI;
									answerUI = std::make_shared<AnswerUI>();
									answerUI->Init();
									answerUI->SetCardKeyLocation(spCardLocation);
									answerUI->SetSecretFileLocation(spFileLocation);
									SceneManager::Instance().AddObject(answerUI);

									std::shared_ptr<MapUpdateUIBack>mapUpdUIback;
									mapUpdUIback = std::make_shared<MapUpdateUIBack>();
									mapUpdUIback->Init();
									SceneManager::Instance().AddObject(mapUpdUIback);

									std::shared_ptr<MapUpdateUI>	mapUpdUI;
									mapUpdUI = std::make_shared<MapUpdateUI>();
									mapUpdUI->Init();
									SceneManager::Instance().AddObject(mapUpdUI);
								}
								else
								{
									std::shared_ptr<AnswerUIBack>	answerUIBack;
									answerUIBack = std::make_shared<AnswerUIBack>();
									answerUIBack->Init();
									SceneManager::Instance().AddObject(answerUIBack);

									std::shared_ptr<NoAnswerUI>		noAnswerUI;
									noAnswerUI = std::make_shared<NoAnswerUI>();
									noAnswerUI->Init();
									SceneManager::Instance().AddObject(noAnswerUI);
								}
							}
						}

						if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy3Flg))
						{
							if (spEnemy_3)
							{
								if (!spEnemy_3->GetAnswerFlg())
								{
									spEnemy_3->Answer();

									std::shared_ptr<AnswerUIBack>	answerUIBack;
									answerUIBack = std::make_shared<AnswerUIBack>();
									answerUIBack->Init();
									SceneManager::Instance().AddObject(answerUIBack);

									std::shared_ptr<AnswerUI>		answerUI;
									answerUI = std::make_shared<AnswerUI>();
									answerUI->Init();
									answerUI->SetCardKeyLocation(spCardLocation);
									answerUI->SetSecretFileLocation(spFileLocation);
									SceneManager::Instance().AddObject(answerUI);

									std::shared_ptr<MapUpdateUIBack>mapUpdUIback;
									mapUpdUIback = std::make_shared<MapUpdateUIBack>();
									mapUpdUIback->Init();
									SceneManager::Instance().AddObject(mapUpdUIback);

									std::shared_ptr<MapUpdateUI>	mapUpdUI;
									mapUpdUI = std::make_shared<MapUpdateUI>();
									mapUpdUI->Init();
									SceneManager::Instance().AddObject(mapUpdUI);
								}
								else
								{
									std::shared_ptr<AnswerUIBack>	answerUIBack;
									answerUIBack = std::make_shared<AnswerUIBack>();
									answerUIBack->Init();
									SceneManager::Instance().AddObject(answerUIBack);

									std::shared_ptr<NoAnswerUI>		noAnswerUI;
									noAnswerUI = std::make_shared<NoAnswerUI>();
									noAnswerUI->Init();
									SceneManager::Instance().AddObject(noAnswerUI);
								}
							}
						}
					}
				}
				// 離されたとき
				else
				{
					// 尋問キーフラグをfalseにする
					_owner.m_bitsKeyFlg.set(KeyFlgType::InterKey, false);
				}
			}
		}
	}
	else
	{
		if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy1Flg))
		{
			_owner.m_bitsEachFlg.set(RestEnemy1Flg, false);

			if (spEnemy_1)
			{
				spEnemy_1->ChangeIdle();
			}
		}

		if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy2Flg))
		{
			_owner.m_bitsEachFlg.set(RestEnemy2Flg, false);

			if (spEnemy_2)
			{
				spEnemy_2->ChangeIdle();
			}
		}

		if (_owner.m_bitsEachFlg.test(EachFlgType::RestEnemy3Flg))
		{
			_owner.m_bitsEachFlg.set(RestEnemy3Flg, false);

			if (spEnemy_3)
			{
				spEnemy_3->ChangeIdle();
			}
		}

		// ActionIdleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}
}

void Player::ActionRestraint_Idle::Exit(Player& _owner)
{
	const std::shared_ptr<RestraintUI>	spRestraintUI	= _owner.m_wpRestraintUI.lock();

	// アニメーションフレームを初期化
	_owner.m_nowAnimeFrm = 0.0f;

	spRestraintUI->SetRestraintFlg(false);
}

void Player::ActionKill::Enter(Player& _owner)
{
	// プレイヤーの状態が"処刑状態"でないとき
	if (_owner.m_sType != Player::SituationType::Kill)
	{
		// プレイヤーの状態を"処刑状態"にし、
		// アニメーションを"Kill"に切り替え
		_owner.m_sType = Player::SituationType::Kill;
		_owner.ChangeAnimation("Kill");
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;
	}
}

void Player::ActionKill::Update(Player& _owner)
{
	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	if (_owner.m_nowAnimeFrm >= _owner.m_exeFrm)
	{
		if (!_owner.m_bitsEachFlg.test(EachFlgType::ExeFlg))
		{
			_owner.m_bitsEachFlg.set(EachFlgType::ExeFlg, true);

			if (_owner.m_spExeSound->IsStopped())
			{
				_owner.m_spExeSound->Play();
			}
		}
	}

	// アニメーションが終了していたら
	if (_owner.m_nowAnimeFrm >= _owner.m_killFrmMax)
	{
		// ActionIdleに切り替え
		_owner.ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}
}

void Player::ActionKill::Exit(Player& _owner)
{
	// アニメーションが終了していたら
	if (_owner.m_nowAnimeFrm >= _owner.m_killFrmMax)
	{
		// アニメーションフレームを初期化
		_owner.m_nowAnimeFrm = 0.0f;
	}

	if (_owner.m_bitsEachFlg.test(EachFlgType::ExeFlg))
	{
		_owner.m_bitsEachFlg.set(EachFlgType::ExeFlg, false);
	}
}

void Player::ActionReload_Idle::Enter(Player& _owner)
{
	// プレイヤーの状態が"リロード状態"でないとき
	if (_owner.m_sType != Player::SituationType::Reload)
	{
		// プレイヤーの状態を"リロード状態"にし、
		// アニメーションを"Reload"に切り替え
		_owner.m_sType = Player::SituationType::Reload;
		_owner.ChangeAnimation("Reload", false);
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;

		_owner.m_spReloadSound->Play();
	}
	// "リロード状態"であるとき
	else
	{
		// 現在のアニメーションフレームが0.0fでないとき
		if (_owner.m_nowAnimeFrm != 0.0f)
		{
			// アニメーションを"Reload"に切り替え
			_owner.ChangeAnimation("Reload", false, _owner.m_nowAnimeFrm);
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;
		}
		// 0.0fであるとき
		else
		{
			// アニメーションを初期化し、
			// アニメーションを"Reload"に切り替え
			_owner.m_nowAnimeFrm = 0.0f;
			_owner.ChangeAnimation("Reload", false);
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;

			_owner.m_spReloadSound->Play();
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
		// アニメーションを"Reload_Walk"に切り替え
		_owner.m_sType = Player::SituationType::Reload;
		_owner.ChangeAnimation("Reload_Walk", false);
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;

		_owner.m_spReloadSound->Play();
		_owner.m_spWalkSound->Play(true);
	}
	// "リロード状態"であるとき
	else
	{
		// 現在のアニメーションフレームが0.0fでないとき
		if (_owner.m_nowAnimeFrm != 0.0f)
		{
			// アニメーションを"Reload_Walk"に切り替え
			_owner.ChangeAnimation("Reload_Walk", false, _owner.m_nowAnimeFrm);
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;
		}
		// 0.0fであるとき
		else
		{
			// 現在のアニメーションフレームを初期化し、
			// アニメーションを"Reload_Walk"に切り替え
			_owner.m_nowAnimeFrm = 0.0f;
			_owner.ChangeAnimation("Reload_Walk", false);
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;

			_owner.m_spReloadSound->Play();
			_owner.m_spWalkSound->Play(true);
		}
	}
}

void Player::ActionReload_Walk::Update(Player& _owner)
{
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<Reticle>				spReticle		= _owner.m_wpReticle.lock();
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
		// アニメーションを"Reload_Run"に切り替え
		_owner.m_sType = Player::SituationType::Reload;
		_owner.ChangeAnimation("Reload_Run", false);
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;

		_owner.m_spReloadSound->Play();
		_owner.m_spRunSound->Play(true);
	}
	// "リロード状態"であるとき
	else
	{
		// 現在のアニメーションフレームが0.0fでないとき
		if (_owner.m_nowAnimeFrm <= _owner.m_reloadFrmMax)
		{
			// アニメーションを"Reload_Run"に切り替え
			_owner.ChangeAnimation("Reload_Run", false, _owner.m_nowAnimeFrm);
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;
		}
		// 0.0fであるとき
		else
		{
			// 現在のアニメーションフレームを初期化し、
			// アニメーションを"Reload_Walk"に切り替え
			_owner.m_nowAnimeFrm = 0.0f;
			_owner.ChangeAnimation("Reload_Run", false);
			_owner.m_animeSpeed = _owner.m_normalAnimSpd;

			_owner.m_spReloadSound->Play();
			_owner.m_spRunSound->Play(true);
		}
	}
}

void Player::ActionReload_Run::Update(Player& _owner)
{
	const std::shared_ptr<Player_Ready_Pistol>	spReady_Pistol	= _owner.m_wpPlayer_Ready_Pistol.lock();
	const std::shared_ptr<Reticle>				spReticle		= _owner.m_wpReticle.lock();
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

void Player::ActionDeath::Enter(Player& _owner)
{
	// プレイヤーの状態が"死亡状態"でないとき
	if (_owner.m_sType != Player::SituationType::Death)
	{
		// プレイヤーの状態を"死亡状態"にし、
		// アニメーションを"Death"に切り替え
		_owner.m_sType = Player::SituationType::Death;
		_owner.ChangeAnimation("Death", false);
		_owner.m_animeSpeed = _owner.m_normalAnimSpd;
	}
}

void Player::ActionDeath::Update(Player& _owner)
{
	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// アニメーションフレームが最後まで再生されていたら
	if (_owner.m_nowAnimeFrm >= _owner.m_deathFrmMax)
	{
		_owner.m_spDeathSound->Play();

		_owner.m_bitsEachFlg.set(EachFlgType::FeedOutFlg, true);
		_owner.m_bitsEachFlg.set(EachFlgType::DeathFlg, true);
	}
}
//================================================================================================================================
// ステートパターン管理系
//================================================================================================================================
