﻿#include "Enemy_3.h"

#include "Enemy_Main_3/Enemy_Main_3.h"
#include "Enemy_Gun_3/Enemy_Gun_3.h"
#include "Enemy_Gun_NoMagazine_3/Enemy_Gun_NoMagazine_3.h"
#include "Enemy_Magazine_3/Enemy_Magazine_3.h"

#include "../../../EnemyBullet/EnemyBullet.h"

#include "../../Player/Player.h"

#include "../../../Effect/MuzzleFlash/MuzzleFlash.h"

#include "../../../UI/ExclamationMarkUI/ExclamationMarkUI.h"

#include "../../../../Scene/SceneManager.h"
#include "../../../../main.h"

void Enemy_3::Init()
{
	m_spAnimator	= std::make_shared<KdAnimator>();
	ChangeActionState(std::make_shared<ActionIdle>());
	ChangeAnimation("Idle");

	m_spRunSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Run.wav");
	m_spRunSound->SetVolume(m_runVol);

	m_spWalkSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Walk.wav");
	m_spWalkSound->SetVolume(m_walkVol);

	m_spReadySound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Enemy_Ready.wav");
	m_spReadySound->SetVolume(m_readyVol);

	m_spShotSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Enemy_Shot.wav");
	m_spShotSound->SetVolume(m_shotVol);

	m_spMgznSetSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Magazine_Set.wav");
	m_spMgznSetSound->SetVolume(m_mgznSetVol);

	m_spMgznRlsSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Magazine_Release.wav");
	m_spMgznRlsSound->SetVolume(m_mgznRlsVol);

	m_spDiscoverSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/Game/Discover.wav");
	m_spDiscoverSound->SetVolume(m_discoverVol);

	m_spDeathSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/Death.wav");
	m_spDeathSound->SetVolume(m_deathVol);

	m_mMuzzle		= Math::Matrix::CreateTranslation(m_muzzlePos);
	m_mlocalAresst	= Math::Matrix::CreateTranslation(m_localAresstPos);

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}

	m_pos			= { 22.0f,-0.9f,0.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.05f;

	m_actionCnt		= 0;
	m_shotWait		= 0;
	m_dissolveCnt	= 0.0f;
	m_nowAnimeFrm	= 0.0f;

	m_objectType	= ObjectType::TypeEnemy_3;

	CharaBase::Init();

	m_pCollider = std::make_unique<KdCollider>();
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Enemy_3::Update()
{
	// デバッグ情報表示 //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

	//KdCollider::SphereInfo sphere1;
	//sphere1.m_sphere.Center = m_pos;
	//sphere1.m_sphere.Radius = m_discoverArea;
	//sphere1.m_type = KdCollider::TypeBump;

	//m_pDebugWire->AddDebugSphere(sphere1.m_sphere.Center, sphere1.m_sphere.Radius, kRedColor);

	//KdCollider::SphereInfo sphere2;
	//sphere2.m_sphere.Center = m_pos;
	//sphere2.m_sphere.Radius = m_waryArea;
	//sphere2.m_type = KdCollider::TypeBump;

	//m_pDebugWire->AddDebugSphere(sphere2.m_sphere.Center, sphere2.m_sphere.Radius, { 1.0f,1.0f,0.0f });

	//KdCollider::SphereInfo sphere3;
	//sphere3.m_sphere.Center = m_pos;
	//sphere3.m_sphere.Radius = m_shotArea;
	//sphere3.m_type = KdCollider::TypeBump;

	//m_pDebugWire->AddDebugSphere(sphere3.m_sphere.Center, sphere3.m_sphere.Radius, { 1.0f,1.0f,1.0f });

	//Math::Vector3 debugPos1 = { 0.0f,-1.0f,-27.0f };
	//KdCollider::SphereInfo sphere4;
	//sphere4.m_sphere.Center = debugPos1;
	//sphere4.m_sphere.Radius = 1.0f;
	//sphere4.m_type = KdCollider::TypeBump;

	//m_pDebugWire->AddDebugSphere(sphere4.m_sphere.Center, sphere4.m_sphere.Radius, { 0.0f,0.0f,1.0f });

	//Math::Vector3 debugPos2 = { 0.0f,-1.0f,27.0f };
	//KdCollider::SphereInfo sphere5;
	//sphere5.m_sphere.Center = debugPos2;
	//sphere5.m_sphere.Radius = 1.0f;
	//sphere5.m_type = KdCollider::TypeBump;

	//m_pDebugWire->AddDebugSphere(sphere5.m_sphere.Center, sphere5.m_sphere.Radius, { 0.0f,0.0f,1.0f });

	//Math::Vector3 debugPos3 = (m_mMuzzle * m_mWorld).Translation();
	//KdCollider::SphereInfo sphere6;
	//sphere6.m_sphere.Center = debugPos3;
	//sphere6.m_sphere.Radius = 0.05f;
	//sphere6.m_type = KdCollider::TypeBump;

	//m_pDebugWire->AddDebugSphere(sphere6.m_sphere.Center, sphere6.m_sphere.Radius, { 1.0f,1.0f,1.0f });

	//Application::Instance().m_log.Clear();
	//Application::Instance().m_log.AddLog("m_lostFlg=%d\n", m_lostFlg);
	//Application::Instance().m_log.AddLog("m_lostCnt=%d\n", m_lostCnt);
	//Application::Instance().m_log.AddLog("m_deathFlg=%d\n", m_deathFlg);
	//Application::Instance().m_log.AddLog("m_dissolveFlg=%d\n", m_dissolveFlg);

	// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////


	SearchProc();

	m_pos.y -= m_gravity;
	m_gravity += 0.04f;	

	// 各種「状態」に応じた更新処理を実行する
	if (m_nowState)
	{
		m_nowState->Update(*this);
	}
}

void Enemy_3::PostUpdate()
{
	m_spReadySound->SetPos(GetPos());
	m_spRunSound->SetPos(GetPos());
	m_spShotSound->SetPos(GetPos());
	m_spWalkSound->SetPos(GetPos());
	m_spMgznSetSound->SetPos(GetPos());
	m_spMgznRlsSound->SetPos(GetPos());
	m_spDeathSound->SetPos(GetPos());

	UpdateCollision();
}

void Enemy_3::UpdateCollision()
{
	//==========================================================
	// レイ判定
	//==========================================================

	KdCollider::RayInfo ray;
	ray.m_pos = m_pos;
	ray.m_dir = Math::Vector3::Down;
	ray.m_pos.y += 0.01f;

	float enableStepHigh = 0.2f;
	ray.m_pos.y += enableStepHigh;

	ray.m_range = m_gravity + enableStepHigh;
	ray.m_type = KdCollider::TypeGround;

	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(ray, &retRayList);
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
	sphere.m_sphere.Center = m_pos + Math::Vector3{ 0.f,0.9f,0.f };
	sphere.m_sphere.Radius = 0.3f;
	sphere.m_type = KdCollider::TypeGround;

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
	maxOverLap = 0.f;
	isHit = false;
	Math::Vector3 hitDir;
	for (auto& ret : retSphereList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			isHit = true;
		}
	}
	if (isHit)
	{
		hitDir.Normalize();

		m_pos += hitDir * maxOverLap;
	}
}

void Enemy_3::ChangeAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	const std::shared_ptr<Enemy_Main_3>			spEnemy_Main				= m_wpEnemy_Main.lock();
	const std::shared_ptr<Enemy_Gun_3>			spEnemy_Gun					= m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Gun_NoMagazine_3>spEnemy_Gun_NoMagazine		= m_wpEnemy_Gun_NoMagazine.lock();
	const std::shared_ptr<Enemy_Magazine_3>		spEnemy_Magazine			= m_wpEnemy_Magazine.lock();

	if (spEnemy_Main)
	{
		spEnemy_Main->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spEnemy_Gun)
	{
		spEnemy_Gun->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spEnemy_Gun_NoMagazine)
	{
		spEnemy_Gun_NoMagazine->ChangeAnimation(_animeName, _isLoop, _time);
	}
	if (spEnemy_Magazine)
	{
		spEnemy_Magazine->ChangeAnimation(_animeName, _isLoop, _time);
	}
}

//=====================================================================================================
// 索敵処理
//=====================================================================================================
void Enemy_3::SearchProc()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	Math::Vector3 targetPos;
	if (spPlayer)
	{
		// プレイヤーの座標を取得
		targetPos = spPlayer->GetPos();
	}

	// プレイヤーと敵の座標の差ベクトルを計算
	Math::Vector3 targetVec = targetPos - m_pos;

	// 敵の方向ベクトルと差ベクトルの内積を計算し格納
	float innerView = m_moveDir.Dot(targetVec);

	// 視野の範囲内のとき
	if (innerView > m_viewRange)
	{
		// 発見範囲内であれば
		if (targetVec.Length() < m_discoverArea)
		{
			// 発見フラグをtrueにする
			m_bitsEachFlg.set(FindFlg,true);
		}
	}
}

//=====================================================================================================
// 追従処理
//=====================================================================================================
void Enemy_3::ChaseProc()
{
	const std::shared_ptr<Player> _spPlayer = m_wpPlayer.lock();
	Math::Vector3 _targetPos;
	if (_spPlayer)
	{
		_targetPos = _spPlayer->GetPos();
	}
	if (m_bitsEachFlg.test(FindFlg))
	{
		m_moveDir = _targetPos - m_pos;
	}

	//// 移動方向を正規化
	//m_moveDir.Normalize();

	//// レイ判定用の辺巣を作成
	//KdCollider::RayInfo ray;

	//// レイの発射位置(座標)を設定
	//ray.m_pos = m_pos + Math::Vector3{ 0.0f,1.0f,0.0f };

	//// レイの発射方向を設定
	//ray.m_dir = m_moveDir;

	//// レイの長さを設定
	//ray.m_range = 15.0f;

	//// 判定をしたいオブジェクトのタイプを設定
	//ray.m_type = KdCollider::TypeBump;

	//// デバッグ表示
	//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range, kBlueColor);

	//// レイに当たったオブジェクト情報
	//std::list<KdCollider::CollisionResult> retRayList;

	//bool isHit = false;		// 当たっていたらtrue
	//float maxOverLap = 0.0f;// はみ出たレイの長さ

	//// レイと当たり判定
	//for (auto& obj : SceneManager::Instance().GetObjList())
	//{
	//	// 当たっているオブジェクトがプレイヤーのとき
	//	if (obj->GetObjectType() == KdGameObject::ObjectType::TypePlayer)
	//	{
	//		m_isPlayer = true;
	//		obj->Intersects(ray, &retRayList);
	//	}
	//	else if(obj->GetObjectType() == KdGameObject::ObjectType::TypeObstacles)
	//	{
	//		m_isPlayer = false;
	//		obj->Intersects(ray, &retRayList);
	//	}
	//}

	//for (auto& ret : retRayList)
	//{
	//	if (maxOverLap < ret.m_overlapDistance)
	//	{
	//		maxOverLap = ret.m_overlapDistance;
	//		isHit = true;
	//	}
	//}

	//if (isHit)
	//{
	//	if (m_isPlayer)
	//	{
	//		m_findFlg = true;
	//		m_lostFlg = false;
	//	}
	//	else
	//	{
	//		if (m_findFlg)
	//		{
	//			m_lostFlg = true;
	//		}
	//	}
	//}
	//else
	//{
	//	if (m_findFlg)
	//	{
	//		m_lostFlg = true;
	//	}
	//}
}
void Enemy_3::LostProc()
{
	const std::shared_ptr<Player>		spPlayer = m_wpPlayer.lock();

	// プレイヤー座標取得用
	Math::Vector3 targetPos;

	// プレイヤーの情報があれば
	if (spPlayer)
	{
		// プレイヤーの座標を取得
		targetPos = spPlayer->GetPos();
	}

	// プレイヤーと敵の座標の差ベクトルを取得
	Math::Vector3 targetVec = targetPos - m_pos;

	// 差ベクトルの長さが警戒範囲外のとき
	if (targetVec.Length() > m_waryArea)
	{
		// 失踪フラグがfalseのとき
		if (!m_bitsEachFlg.test(LostFlg))
		{
			// 失踪フラグをtrueにする
			m_bitsEachFlg.set(LostFlg, true);
		}
	}
	else
	{
		// 失踪フラグがtrueのとき
		if (m_bitsEachFlg.test(LostFlg))
		{
			// 失踪フラグをfalseにする
			m_bitsEachFlg.set(LostFlg,false);
		}
	}

	// 失踪フラグがtrueのとき
	if (m_bitsEachFlg.test(LostFlg))
	{
		// 失踪カウントを増やす
		m_lostCnt++;
	}
	else
	{
		// 失踪カウントが最小値ではないとき
		if (m_lostCnt != m_lostCntMin)
		{
			// 失踪カウントを最小値に設定
			m_lostCnt = m_lostCntMin;
		}
	}

	// 失踪カウントが最大値以上のとき
	if (m_lostCnt >= m_lostCntMax)
	{
		// 発見フラグがtrueのとき
		if (m_bitsEachFlg.test(FindFlg))
		{
			// 発見フラグをfalseにする
			m_bitsEachFlg.set(FindFlg, false);
		}

		// 失踪カウントを最大値に設定
		m_lostCnt = m_lostCntMax;

		// "ActionIdle"に切り替え
		ChangeActionState(std::make_shared<ActionIdle>());
		return;
	}
}

void Enemy_3::ChangeAressted()
{
	// ActionAresstedに切り替え
	ChangeActionState(std::make_shared<ActionAressted>());
	return;
}

void Enemy_3::Execution()
{
	// ActionArested_Deathに切り替え
	ChangeActionState(std::make_shared<ActionAressted_Death>());
	return;
}

void Enemy_3::ChangeIdle()
{
	// ActionIdleに切り替え
	ChangeActionState(std::make_shared<ActionIdle>());
	return;
}

void Enemy_3::SetDeathFlg(const bool _deathFlg)
{
	// 死亡フラグをtrueにする
	m_bitsEachFlg.set(DeathFlg, _deathFlg);
}

void Enemy_3::ChangeActionState(std::shared_ptr<ActionStateBase> _nextState)
{
	if (m_nowState)m_nowState->Exit(*this);
	m_nowState = _nextState;
	m_nowState->Enter(*this);
}

void Enemy_3::ActionIdle::Enter(Enemy_3& _owner)
{
	// 敵の状態が"停止状態"でないとき
	if (_owner.m_sitType != SituationType::Idle)
	{
		// 敵の状態を"停止状態"に切り替え、
		// アニメーションを"Idle"に切り替える
		_owner.m_sitType = SituationType::Idle;
		_owner.ChangeAnimation("Idle");
	}

	// アクションカウントを初期化
	_owner.m_actionCnt = 0;
}

void Enemy_3::ActionIdle::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Enemy_Gun_3>	spGun = _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main_3>	spEnemy_Main = _owner.m_wpEnemy_Main.lock();

	// 銃の情報があれば
	if (spGun)
	{
		// 残弾数が0以下であれば
		if (spGun->GetNowBullet() <= 0)
		{
			// ActionReloadに切り替え
			_owner.ChangeActionState(std::make_shared<ActionReload>());
			return;
		}
	}

	// 移動方向を設定
	_owner.m_moveDir = Math::Vector3::Zero;

	// 敵がプレイヤーを見つけている状態のとき
	if (_owner.m_bitsEachFlg.test(FindFlg))
	{
		// ステートを"走行状態"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDiscover>());
		return;
	}
	// 見つけていないとき
	else
	{
		// アクションカウントを増やす
		_owner.m_actionCnt++;

		// アクションカウントが180以上とき
		if (_owner.m_actionCnt >= 180)
		{
			// "ActionWalk"に切り替え
			_owner.ChangeActionState(std::make_shared<ActionWalk>());
			return;
		}
	}

	// 死亡フラグがtrueのとき
	if (_owner.m_bitsEachFlg.test(DeathFlg))
	{
		// "ActionDeath"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDeath>());
		return;
	}

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}

void Enemy_3::ActionWalk::Enter(Enemy_3& _owner)
{
	// 敵の状態が"歩行状態"でないとき
	if (_owner.m_sitType != SituationType::Walk)
	{
		// 敵の状態を"歩行状態"に切り替え、
		// アニメーションを"Walk"に切り替え
		_owner.m_sitType = SituationType::Walk;
		_owner.ChangeAnimation("Walk");

		_owner.m_spWalkSound->Play(true);
	}
}

void Enemy_3::ActionWalk::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Enemy_Gun_3>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main_3>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();

	// 銃の情報があれば
	if (spGun)
	{
		// 残弾数が0以下であれば
		if (spGun->GetNowBullet() <= 0)
		{
			// ActionReloadに切り替え
			_owner.ChangeActionState(std::make_shared<ActionReload>());
			return;
		}
	}

	// 敵がプレイヤーを見つけている状態のとき
	if (_owner.m_bitsEachFlg.test(FindFlg))
	{
		// ステートを"発見状態"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDiscover>());
		return;
	}

	// 現在の移動速度が歩行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_walkMoveSpd)
	{
		// 移動速度を再設定
		_owner.m_moveSpeed = _owner.m_walkMoveSpd;
	}

	// 方向切り替えフラグがfalseのとき
	if (!_owner.m_bitsEachFlg.test(DirFlg))
	{
		_owner.m_moveDir += Math::Vector3::Backward;
	}
	// 方向切り替えフラグがtrueのとき
	else
	{
		_owner.m_moveDir += Math::Vector3::Forward;
	}

	// 座標が27.0f以上のとき
	if (_owner.m_pos.z >= _owner.m_arrivalZPos)
	{
		// 方向切り替えフラグがfalseのとき
		if (!_owner.m_bitsEachFlg.test(DirFlg))
		{
			// 方向切り替えフラグをtrueにする
			_owner.m_bitsEachFlg.set(DirFlg, true);

			// Z座標を27.0fに固定
			_owner.m_pos.z = _owner.m_arrivalZPos;

			// ActionIdleに切り替え
			_owner.ChangeActionState(std::make_shared<ActionIdle>());
			return;
		}
	}
	// 座標が-27.0f以下のとき
	else if (_owner.m_pos.z <= -_owner.m_arrivalZPos)
	{
		// 方向切り替えフラグがtrueのとき
		if (_owner.m_bitsEachFlg.test(DirFlg))
		{
			// 方向切り替えフラグをfalseにする
			_owner.m_bitsEachFlg.set(DirFlg, false);

			// Z座標を-27.0fに固定
			_owner.m_pos.z = -_owner.m_arrivalZPos;

			// ActionIdleに切り替え
			_owner.ChangeActionState(std::make_shared<ActionIdle>());
			return;
		}
	}

	// 死亡フラグがtrueのとき
	if (_owner.m_bitsEachFlg.test(DeathFlg))
	{
		// "ActionDeath"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDeath>());
		return;
	}

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	//	キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}

void Enemy_3::ActionWalk::Exit(Enemy_3& _owner)
{
	_owner.m_spWalkSound->Stop();
}

void Enemy_3::ActionDiscover::Enter(Enemy_3& _owner)
{
	const std::shared_ptr<Player> spPlayer = _owner.m_wpPlayer.lock();

	std::shared_ptr<ExclamationMarkUI> ExMarkUI;
	ExMarkUI = std::make_shared<ExclamationMarkUI>();
	ExMarkUI->Init();
	ExMarkUI->SetPlayer(spPlayer);
	ExMarkUI->SetPos(_owner.m_pos);
	SceneManager::Instance().AddObject(ExMarkUI);

	// 敵の状態が"発見状態"でないとき
	if (_owner.m_sitType != SituationType::Discover)
	{
		// 敵の状態を"発見状態"に切り替え、
		// アニメーションを"Run"に切り替え
		_owner.m_sitType = SituationType::Discover;
		_owner.ChangeAnimation("Discover", false);

		_owner.m_spDiscoverSound->Play();
	}

	// 現在のアニメーションフレームが0以外であれば
	if (_owner.m_nowAnimeFrm != _owner.m_zeroAnimFrm)
	{
		// 現在のアニメーションフレームを初期化
		_owner.m_nowAnimeFrm = _owner.m_zeroAnimFrm;
	}
}

void Enemy_3::ActionDiscover::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Player> spPlayer = _owner.m_wpPlayer.lock();

	// プレイヤーの情報があるとき
	if (spPlayer)
	{
		// 敵とプレイヤーの差ベクトルを算出
		_owner.m_moveDir = spPlayer->GetPos() - _owner.m_pos;
	}

	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// 現在の移動速度が0でないとき
	if (_owner.m_moveSpeed != _owner.m_zeroMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_zeroMoveSpd;
	}

	// 現在のアニメーションが終了していたら
	if (_owner.m_nowAnimeFrm >= _owner.m_dscovrAnimFrm)
	{
		// ActionRunに切り替え
		_owner.ChangeActionState(std::make_shared<ActionRun>());
		return;
	}

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	// キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}

void Enemy_3::ActionRun::Enter(Enemy_3& _owner)
{
	// 敵の状態が"走行状態"でないとき
	if (_owner.m_sitType != SituationType::Run)
	{
		// 敵の状態を"走行状態"に切り替え、
		// アニメーションを"Run"に切り替え
		_owner.m_sitType = SituationType::Run;
		_owner.ChangeAnimation("Run");

		_owner.m_spRunSound->Play(true);
	}
}

void Enemy_3::ActionRun::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Enemy_Gun_3>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main_3>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Player>		spPlayer		= _owner.m_wpPlayer.lock();

	// 失踪処理
	_owner.LostProc();

	// 銃の情報があれば
	if (spGun)
	{
		// 残弾数が0以下であれば
		if (spGun->GetNowBullet() <= 0)
		{
			// ActionReloadに切り替え
			_owner.ChangeActionState(std::make_shared<ActionReload>());
			return;
		}
	}

	// プレイヤー座標取得用
	Math::Vector3 targetPos;

	// プレイヤーの情報があれば
	if (spPlayer)
	{
		// プレイヤーの座標を取得
		targetPos = spPlayer->GetPos();
	}

	// プレイヤーと敵の座標の差ベクトルを取得
	Math::Vector3 targetVec = targetPos - _owner.m_pos;

	// 差ベクトルの長さが銃発射範囲内のとき
	if (targetVec.Length() < _owner.m_shotArea)
	{
		// "ActionReady"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionReady>());
		return;
	}

	// 死亡フラグがtrueのとき
	if (_owner.m_bitsEachFlg.test(DeathFlg))
	{
		// "ActionDeath"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDeath>());
		return;
	}

	// 現在の移動速度が走行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_walkMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_runMoveSpd;
	}

	// 追尾処理
	_owner.ChaseProc();

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	// キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}

void Enemy_3::ActionRun::Exit(Enemy_3& _owner)
{
	_owner.m_spRunSound->Stop();
}

void Enemy_3::ActionReady::Enter(Enemy_3& _owner)
{
	// 敵の状態が"構え状態"でないとき
	if (_owner.m_sitType != SituationType::Ready)
	{
		// 敵の状態を"構え状態"に切り替え、
		// アニメーションを"Ready"に切り替え
		_owner.m_sitType = SituationType::Ready;
		_owner.ChangeAnimation("Ready");

		_owner.m_spReadySound->Play();
	}

	// アクションカウントを初期化
	_owner.m_actionCnt = 0;
}

void Enemy_3::ActionReady::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Enemy_Gun_3>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main_3>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Player>		spPlayer		= _owner.m_wpPlayer.lock();
	Math::Vector3 targetPos;

	// 失踪処理
	_owner.LostProc();

	// プレイヤーの情報があるとき
	if (spPlayer)
	{
		// プレイヤーの座標を取得
		targetPos = spPlayer->GetPos();
	}

	// 銃の情報があれば
	if (spGun)
	{
		// 残弾数が0以下であれば
		if (spGun->GetNowBullet() <= 0)
		{
			// ActionReloadに切り替え
			_owner.ChangeActionState(std::make_shared<ActionReload>());
			return;
		}
	}

	// 現在の移動速度が走行速度と同じでないとき
	if (_owner.m_moveSpeed != _owner.m_zeroMoveSpd)
	{
		// 移動速度を初期化
		_owner.m_moveSpeed = _owner.m_zeroMoveSpd;
	}

	// アクションカウントを増やす
	_owner.m_actionCnt++;

	// アクションカウントが60以上のとき
	if (_owner.m_actionCnt >= 60)
	{
		_owner.ChangeActionState(std::make_shared<ActionShot>());
		return;
	}

	// プレイヤーの座標と敵の座標の差ベクトルを計算
	Math::Vector3 targetVec = targetPos - _owner.m_pos;

	// 差ベクトルの長さが8.0fより大きいとき
	if (targetVec.Length() > _owner.m_shotArea)
	{
		// ActionRunに切り替え
		_owner.ChangeActionState(std::make_shared<ActionRun>());
		return;
	}

	// 死亡フラグがtrueのとき
	if (_owner.m_bitsEachFlg.test(DeathFlg))
	{
		// "ActionDeath"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDeath>());
		return;
	}

	// 移動方向を設定
	_owner.m_moveDir = targetPos - _owner.m_pos;

	// 移動方向を正規化
	_owner.m_moveDir.Normalize();

	// 移動方向と移動速度を合成し、座標に加算して更新
	_owner.m_pos += _owner.m_moveDir * _owner.m_moveSpeed;

	// キャラクターの回転角度を計算する
	_owner.UpdateRotate(_owner.m_moveDir);

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}

void Enemy_3::ActionShot::Enter(Enemy_3& _owner)
{
	// 敵の状態が"弾発射状態"でないとき
	if (_owner.m_sitType != SituationType::Shot)
	{
		// 敵の状態を"弾発射状態"に切り替え、
		// アニメーションを"Shot"に切り替え
		_owner.m_sitType = SituationType::Shot;
		_owner.ChangeAnimation("Shot");

		_owner.m_spShotSound->Play(true);
	}

	// アクションカウントを初期化
	_owner.m_actionCnt = 0;
}

void Enemy_3::ActionShot::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Enemy_Gun_3>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main_3>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Player>		spPlayer		= _owner.m_wpPlayer.lock();

	Math::Vector3 targetPos;

	// 失踪処理
	_owner.LostProc();

	// プレイヤーの情報があるとき
	if (spPlayer)
	{
		// プレイヤーの座標を取得
		targetPos = spPlayer->GetPos();
	}

	// 銃の情報があれば
	if (spGun)
	{
		// 残弾数が0以下であれば
		if (spGun->GetNowBullet() <= 0)
		{
			// ActionReloadに切り替え
			_owner.ChangeActionState(std::make_shared<ActionReload>());
			return;
		}
	}

	// アクションカウントを増やす
	_owner.m_actionCnt++;

	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// 発射待機時間を減らす
	_owner.m_shotWait--;

	// 発射待機時間が0以下であれば
	if (_owner.m_shotWait <= 0)
	{
		// 発射待機時間を0に固定
		_owner.m_shotWait = 0;
	}

	// 現在のアニメーションフレームが弾発射時のフレーム以上のとき
	if (_owner.m_nowAnimeFrm >= _owner.m_shotAnimFrm / 2)
	{
		// 発射待機時間が0以下であれば
		if (_owner.m_shotWait <= 0)
		{
			// 銃の情報があれば
			if (spGun)
			{
				// 発射待機時間をリセット
				_owner.m_shotWait = _owner.m_maxShotWait;

				// 銃口の座標を取得
				Math::Vector3 muzzlePos = (_owner.m_mMuzzle * _owner.m_mWorld).Translation();

				// 敵の向いている方向を取得
				Math::Vector3 dir = _owner.m_mWorld.Backward();

				// 回転行列を宣言
				Math::Matrix rotMat;

				// 弾にばらつきを持たせる
				float x = DirectX::XMConvertToRadians(static_cast<float>(rand() % 11 - 5));
				float y = DirectX::XMConvertToRadians(static_cast<float>(rand() % 11 - 5));
				float z = DirectX::XMConvertToRadians(static_cast<float>(rand() % 11 - 5));

				rotMat = Math::Matrix::CreateFromYawPitchRoll(y, x, z);
				dir = Math::Vector3::TransformNormal(dir, rotMat);

				// 弾を生成
				std::shared_ptr<EnemyBullet> bullet;
				bullet = std::make_shared<EnemyBullet>();
				bullet->Init();
				bullet->Shot(muzzlePos, dir);
				SceneManager::Instance().AddObject(bullet);

				// マズルフラッシュを生成
				std::shared_ptr<MuzzleFlash> muzzleFlash;
				muzzleFlash = std::make_shared<MuzzleFlash>();
				muzzleFlash->Init();
				muzzleFlash->SetPos(muzzlePos);
				SceneManager::Instance().AddObject(muzzleFlash);

				// 残弾数を減らす
				spGun->BulletDec();
			}
		}
	}

	// プレイヤーの座標と敵の座標の差ベクトルを計算
	Math::Vector3 targetVec = targetPos - _owner.m_pos;

	// 差ベクトルの長さが8.0fより大きいとき
	if (targetVec.Length() > _owner.m_shotArea)
	{
		// ActionRunに切り替え
		_owner.ChangeActionState(std::make_shared<ActionRun>());
		return;
	}

	// アクションカウントが60以上のとき
	if (_owner.m_actionCnt >= 60)
	{
		// 現在のアニメーションフレームが終了していたら
		if (_owner.m_nowAnimeFrm >= _owner.m_shotAnimFrm)
		{
			// ActionReadyに切り替え
			_owner.ChangeActionState(std::make_shared<ActionReady>());
			return;
		}
	}

	// 死亡フラグがtrueのとき
	if (_owner.m_bitsEachFlg.test(DeathFlg))
	{
		// "ActionDeath"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDeath>());
		return;
	}

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}

void Enemy_3::ActionShot::Exit(Enemy_3& _owner)
{
	_owner.m_spShotSound->Stop();
}

void Enemy_3::ActionAressted::Enter(Enemy_3& _owner)
{
	// 敵の状態が"拘束状態"でないとき
	if (_owner.m_sitType != SituationType::Aressted)
	{
		// 敵の状態を"拘束状態"に切り替え、
		// アニメーションを"Aressted"に切り替え
		_owner.m_sitType = SituationType::Aressted;
		_owner.ChangeAnimation("Aressted");
	}
}

void Enemy_3::ActionAressted::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Player> spPlayer = _owner.m_wpPlayer.lock();

	// 発見フラグがfalseのとき
	if (!_owner.m_bitsEachFlg.test(FindFlg))
	{
		// 発見フラグをtrueにする
		_owner.m_bitsEachFlg.set(FindFlg, true);
	}

	// プレイヤーの情報があるとき
	if (spPlayer)
	{
		// 行列作成
		_owner.m_mWorld = _owner.m_mlocalAresst * spPlayer->GetMatrix();
	}
}

void Enemy_3::ActionAressted_Death::Enter(Enemy_3& _owner)
{
	// 敵の状態が"拘束状態"でないとき
	if (_owner.m_sitType != SituationType::Aressted_Death)
	{
		// 敵の状態を"拘束状態"に切り替え、
		// アニメーションを"Aressted"に切り替え
		_owner.m_sitType = SituationType::Aressted_Death;
		_owner.ChangeAnimation("Aressted_Death", false);
	}

	// アニメーションフレームが0でないとき
	if (_owner.m_nowAnimeFrm != _owner.m_zeroAnimFrm)
	{
		// アニメーションフレームを初期化
		_owner.m_nowAnimeFrm = _owner.m_zeroAnimFrm;
	}
}

void Enemy_3::ActionAressted_Death::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Player> spPlayer = _owner.m_wpPlayer.lock();

	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// アニメーションが終了していれば
	if (_owner.m_nowAnimeFrm >= _owner.m_exeAnimFrm)
	{
		_owner.m_bitsEachFlg.set(DissolveFlg, true);
	}

	// ディゾルブフラグがtrueのとき
	if (_owner.m_bitsEachFlg.test(DissolveFlg))
	{
		// ディゾルブカウントを進める
		_owner.m_dissolveCnt += _owner.m_dissolveSpd;
	}

	// ディゾルブカウントがMAX値以上のとき
	if (_owner.m_dissolveCnt >= _owner.m_dissolveMax)
	{
		// ディゾルブカウントをMAX値に固定し、
		// クラスを破棄する
		_owner.m_dissolveCnt = _owner.m_dissolveMax;
		_owner.m_isExpired = true;
	}

	// プレイヤーの情報があるとき
	if (spPlayer)
	{
		// 行列作成
		_owner.m_mWorld = _owner.m_mlocalAresst * spPlayer->GetMatrix();
	}
}

void Enemy_3::ActionReload::Enter(Enemy_3& _owner)
{
	// 敵の状態が"リロード状態"でないとき
	if (_owner.m_sitType != SituationType::Reload)
	{
		// 敵の状態を"リロード状態"に切り替え、
		// アニメーションを"Reload"に切り替え
		_owner.m_sitType = SituationType::Reload;
		_owner.ChangeAnimation("Reload", false);

		_owner.m_spMgznSetSound->Play();
	}

	// 現在のアニメーションフレームが0以外であれば
	if (_owner.m_nowAnimeFrm != _owner.m_zeroAnimFrm)
	{
		// 現在のアニメーションフレームを初期化
		_owner.m_nowAnimeFrm = _owner.m_zeroAnimFrm;
	}

}

void Enemy_3::ActionReload::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Enemy_Main_3>				spMain				= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Enemy_Gun_3>				spGun				= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Magazine_3>			spMagazine			= _owner.m_wpEnemy_Magazine.lock();
	const std::shared_ptr<Enemy_Gun_NoMagazine_3>	spGun_NoMagazine	= _owner.m_wpEnemy_Gun_NoMagazine.lock();

	// 失踪処理
	_owner.LostProc();

	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	if (_owner.m_nowAnimeFrm >= 6.0f && _owner.m_nowAnimeFrm <= 6.1f)
	{
		spGun->SetAlpha(_owner.m_alphaMin);
		spMagazine->SetAlpha(_owner.m_alphaMax);
		spGun_NoMagazine->SetAlpha(_owner.m_alphaMax);
	}

	if (_owner.m_nowAnimeFrm >= 48.0f && _owner.m_nowAnimeFrm <= 48.1f)
	{
		spGun->SetAlpha(_owner.m_alphaMax);
		spMagazine->SetAlpha(_owner.m_alphaMin);
		spGun_NoMagazine->SetAlpha(_owner.m_alphaMin);
	}

	// アニメーションが終了していれば
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadAnimFrm)
	{
		// 銃の情報があれば
		if (spGun)
		{
			// リロードする
			spGun->Reload();

			// 発見フラグがtrueのとき
			if (_owner.m_bitsEachFlg.test(FindFlg))
			{
				// ActionReadyに切り替え
				_owner.ChangeActionState(std::make_shared<ActionReady>());
				return;
			}
			else
			{
				// ActionIdleに切り替え
				_owner.ChangeActionState(std::make_shared<ActionIdle>());
				return;
			}
		}
	}

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}

void Enemy_3::ActionReload::Exit(Enemy_3& _owner)
{
	_owner.m_spMgznRlsSound->Play();
}

void Enemy_3::ActionDeath::Enter(Enemy_3& _owner)
{
	// 敵の状態が"死亡状態"でないとき
	if (_owner.m_sitType != SituationType::Death)
	{
		// 敵の状態を"死亡状態"に切り替え、
		// アニメーションを"Death"に切り替え
		_owner.m_sitType = SituationType::Death;
		_owner.ChangeAnimation("Death", false);
	}

	// 現在のアニメーションフレームが0以外であれば
	if (_owner.m_nowAnimeFrm != _owner.m_zeroAnimFrm)
	{
		// 現在のアニメーションフレームを初期化
		_owner.m_nowAnimeFrm = _owner.m_zeroAnimFrm;
	}
}

void Enemy_3::ActionDeath::Update(Enemy_3& _owner)
{
	const std::shared_ptr<Enemy_Main_3>				spEnemy_Main			= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Enemy_Gun_3>				spEnemy_Gun				= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Gun_NoMagazine_3>	spEnemy_Gun_NoMagazine	= _owner.m_wpEnemy_Gun_NoMagazine.lock();
	const std::shared_ptr<Enemy_Magazine_3>			spEnemy_Magazine		= _owner.m_wpEnemy_Magazine.lock();


	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// アニメションが転倒アニメーションフレームのとき
	if (_owner.m_nowAnimeFrm >= _owner.m_downAnimFrm && _owner.m_nowAnimeFrm <= _owner.m_downAnimFrm + 0.1f)
	{
		if (_owner.m_spDeathSound->IsStopped())
		{
			_owner.m_spDeathSound->Play();
		}
	}

	// アニメーションが終了していれば
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadAnimFrm)
	{
		_owner.m_spDeathSound->Stop();
		_owner.m_bitsEachFlg.set(DissolveFlg, true);
	}

	// ディゾルブフラグがtrueのとき
	if (_owner.m_bitsEachFlg.test(DissolveFlg))
	{
		// ディゾルブカウントを進める
		_owner.m_dissolveCnt += _owner.m_dissolveSpd;
	}

	// ディゾルブカウントがMAX値以上のとき
	if (_owner.m_dissolveCnt >= _owner.m_dissolveMax)
	{
		// ディゾルブカウントをMAX値に固定し、
		// クラスを破棄する
		_owner.m_dissolveCnt	= _owner.m_dissolveMax;
		_owner.m_isExpired		= true;
	}

	_owner.m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(_owner.m_angle.y));
	_owner.m_mTrans = Math::Matrix::CreateTranslation(_owner.m_pos);
	_owner.m_mWorld = _owner.m_mRot * _owner.m_mTrans;
}
