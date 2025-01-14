#include "Enemy.h"

#include "Enemy_Main/Enemy_Main.h"
#include "Enemy_Gun/Enemy_Gun.h"
#include "Enemy_Gun_NoMagazine/Enemy_Gun_NoMagazine.h"
#include "Enemy_Magazine/Enemy_Magazine.h"

#include "../Player/Player.h"

#include "../../../Scene/SceneManager.h"
#include "../../../main.h"

void Enemy::Init()
{
	m_spAnimator	= std::make_shared<KdAnimator>();
	ChangeActionState(std::make_shared<ActionIdle>());
	ChangeAnimation("Idle");

	m_dirFlg		= false;
	m_waryFlg		= false;
	m_lostFlg		= false;
	m_findFlg		= false;
	m_deathFlg		= false;
	m_dissolveFlg	= false;

	m_pos			= { 0.f,-0.9f,-27.0f };
	m_moveDir		= Math::Vector3::Zero;
	m_moveSpeed		= 0.05f;

	m_actionCnt		= 0;
	m_shotWait		= 0;
	m_dissolveCnt	= 0.0f;
	m_nowAnimeFrm	= 0.0f;

	m_objectType	= ObjectType::TypeEnemy;

	CharaBase::Init();

	m_pCollider = std::make_unique<KdCollider>();
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Enemy::Update()
{
	// デバッグ情報表示 //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

	KdCollider::SphereInfo sphere1;
	sphere1.m_sphere.Center = m_pos;
	sphere1.m_sphere.Radius = m_discoverArea;
	sphere1.m_type = KdCollider::TypeBump;

	m_pDebugWire->AddDebugSphere(sphere1.m_sphere.Center, sphere1.m_sphere.Radius, kRedColor);

	KdCollider::SphereInfo sphere2;
	sphere2.m_sphere.Center = m_pos;
	sphere2.m_sphere.Radius = m_waryArea;
	sphere2.m_type = KdCollider::TypeBump;

	m_pDebugWire->AddDebugSphere(sphere2.m_sphere.Center, sphere2.m_sphere.Radius, { 1.0f,1.0f,0.0f });

	KdCollider::SphereInfo sphere3;
	sphere3.m_sphere.Center = m_pos;
	sphere3.m_sphere.Radius = m_shotArea;
	sphere3.m_type = KdCollider::TypeBump;

	m_pDebugWire->AddDebugSphere(sphere3.m_sphere.Center, sphere3.m_sphere.Radius, { 1.0f,1.0f,1.0f });

	Math::Vector3 debugPos1 = { 0.0f,-1.0f,-27.0f };
	KdCollider::SphereInfo sphere4;
	sphere4.m_sphere.Center = debugPos1;
	sphere4.m_sphere.Radius = 1.0f;
	sphere4.m_type = KdCollider::TypeBump;

	m_pDebugWire->AddDebugSphere(sphere4.m_sphere.Center, sphere4.m_sphere.Radius, { 0.0f,0.0f,1.0f });

	Math::Vector3 debugPos2 = { 0.0f,-1.0f,27.0f };
	KdCollider::SphereInfo sphere5;
	sphere5.m_sphere.Center = debugPos2;
	sphere5.m_sphere.Radius = 1.0f;
	sphere5.m_type = KdCollider::TypeBump;

	m_pDebugWire->AddDebugSphere(sphere5.m_sphere.Center, sphere5.m_sphere.Radius, { 0.0f,0.0f,1.0f });

	Application::Instance().m_log.Clear();
	Application::Instance().m_log.AddLog("m_lostFlg=%d\n", m_lostFlg);
	Application::Instance().m_log.AddLog("m_deathFlg=%d\n", m_deathFlg);
	Application::Instance().m_log.AddLog("m_dissolveFlg=%d\n", m_dissolveFlg);

	// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////


	SearchProc();

	//ChaseProc();

	//RouteSearchProc();

	m_pos.y -= m_gravity;
	m_gravity += 0.04f;	

	// 各種「状態」に応じた更新処理を実行する
	if (m_nowState)
	{
		m_nowState->Update(*this);
	}

	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle.y));
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;
}

void Enemy::PostUpdate()
{
	UpdateCollision();
}

void Enemy::UpdateCollision()
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

void Enemy::ChangeAnimation(const std::string& _animeName, bool _isLoop, float _time)
{
	const std::shared_ptr<Enemy_Main>			spEnemy_Main			= m_wpEnemy_Main.lock();
	const std::shared_ptr<Enemy_Gun>			spEnemy_Gun				= m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Gun_NoMagazine>	spEnemy_Gun_NoMagazine	= m_wpEnemy_Gun_NoMagazine.lock();
	const std::shared_ptr<Enemy_Magazine>		spEnemy_Magazine		= m_wpEnemy_Magazine.lock();

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
// 索敵処理・・・ここから
//=====================================================================================================
void Enemy::SearchProc()
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
			m_findFlg = true;
		}
	}
}
//=====================================================================================================
// 索敵処理・・・ここまで
//=====================================================================================================

//=====================================================================================================
// 追従処理・・・ここから
//=====================================================================================================
void Enemy::ChaseProc()
{
	const std::shared_ptr<Player> _spPlayer = m_wpPlayer.lock();
	Math::Vector3 _targetPos;
	if (_spPlayer)
	{
		_targetPos = _spPlayer->GetPos();
	}
	if (m_findFlg)
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
//=====================================================================================================
// 追従処理・・・ここまで
//=====================================================================================================

void Enemy::Death(const bool _deathFlg)
{
	// 死亡フラグをtrueにする
	m_deathFlg = _deathFlg;
}

void Enemy::ChangeActionState(std::shared_ptr<ActionStateBase> _nextState)
{
	if (m_nowState)m_nowState->Exit(*this);
	m_nowState = _nextState;
	m_nowState->Enter(*this);
}

void Enemy::ActionIdle::Enter(Enemy& _owner)
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

void Enemy::ActionIdle::Update(Enemy& _owner)
{
	const std::shared_ptr<Enemy_Gun>	spGun = _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main>	spEnemy_Main = _owner.m_wpEnemy_Main.lock();

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
	if (_owner.m_findFlg)
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
	if (_owner.m_deathFlg)
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
}

void Enemy::ActionWalk::Enter(Enemy& _owner)
{
	// 敵の状態が"歩行状態"でないとき
	if (_owner.m_sitType != SituationType::Walk)
	{
		// 敵の状態を"歩行状態"に切り替え、
		// アニメーションを"Walk"に切り替え
		_owner.m_sitType = SituationType::Walk;
		_owner.ChangeAnimation("Walk");
	}
}

void Enemy::ActionWalk::Update(Enemy& _owner)
{
	const std::shared_ptr<Enemy_Gun>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();

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
	if (_owner.m_findFlg)
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
	if (!_owner.m_dirFlg)
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
		if (!_owner.m_dirFlg)
		{
			// 方向切り替えフラグをtrueにする
			_owner.m_dirFlg = true;

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
		if (_owner.m_dirFlg)
		{
			// 方向切り替えフラグをfalseにする
			_owner.m_dirFlg = false;

			// Z座標を-27.0fに固定
			_owner.m_pos.z = -_owner.m_arrivalZPos;

			// ActionIdleに切り替え
			_owner.ChangeActionState(std::make_shared<ActionIdle>());
			return;
		}
	}

	// 死亡フラグがtrueのとき
	if (_owner.m_deathFlg)
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
}

void Enemy::ActionDiscover::Enter(Enemy& _owner)
{
	// 敵の状態が"発見状態"でないとき
	if (_owner.m_sitType != SituationType::Discover)
	{
		// 敵の状態を"発見状態"に切り替え、
		// アニメーションを"Run"に切り替え
		_owner.m_sitType = SituationType::Discover;
		_owner.ChangeAnimation("Discover", false);
	}

	// 現在のアニメーションフレームが0以外であれば
	if (_owner.m_nowAnimeFrm != _owner.m_zeroAnimFrm)
	{
		// 現在のアニメーションフレームを初期化
		_owner.m_nowAnimeFrm = _owner.m_zeroAnimFrm;
	}
}

void Enemy::ActionDiscover::Update(Enemy& _owner)
{
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
}

void Enemy::ActionRun::Enter(Enemy& _owner)
{
	// 敵の状態が"走行状態"でないとき
	if (_owner.m_sitType != SituationType::Run)
	{
		// 敵の状態を"走行状態"に切り替え、
		// アニメーションを"Run"に切り替え
		_owner.m_sitType = SituationType::Run;
		_owner.ChangeAnimation("Run");
	}
}

void Enemy::ActionRun::Update(Enemy& _owner)
{
	const std::shared_ptr<Enemy_Gun>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();

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

	// プレイヤーの情報を取得
	std::shared_ptr<Player> spPlayer = _owner.m_wpPlayer.lock();

	// プレイヤー座標取得用
	Math::Vector3 targetPos;

	// プレイヤーの情報があれば
	if (spPlayer)
	{
		// プレイヤーの座標を取得
		targetPos = spPlayer->GetPos();
	}

	// プレイヤーと敵の座標の差ベクトル取得用
	Math::Vector3 targetVec = targetPos - _owner.m_pos;

	// 差ベクトルが銃発射範囲内であれば
	if (targetVec.Length() < _owner.m_shotArea)
	{
		// "ActionReady"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionReady>());
		return;
	}

	// 死亡フラグがtrueのとき
	if (_owner.m_deathFlg)
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
}

void Enemy::ActionReady::Enter(Enemy& _owner)
{
	// 敵の状態が"構え状態"でないとき
	if (_owner.m_sitType != SituationType::Ready)
	{
		// 敵の状態を"構え状態"に切り替え、
		// アニメーションを"Ready"に切り替え
		_owner.m_sitType = SituationType::Ready;
		_owner.ChangeAnimation("Ready");
	}

	// アクションカウントを初期化
	_owner.m_actionCnt = 0;
}

void Enemy::ActionReady::Update(Enemy& _owner)
{
	const std::shared_ptr<Enemy_Gun>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Player>		spPlayer		= _owner.m_wpPlayer.lock();
	Math::Vector3 targetPos;

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
	if (_owner.m_deathFlg)
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
}

void Enemy::ActionShot::Enter(Enemy& _owner)
{
	// 敵の状態が"弾発射状態"でないとき
	if (_owner.m_sitType != SituationType::Shot)
	{
		// 敵の状態を"弾発射状態"に切り替え、
		// アニメーションを"Shot"に切り替え
		_owner.m_sitType = SituationType::Shot;
		_owner.ChangeAnimation("Shot");
	}

	// アクションカウントを初期化
	_owner.m_actionCnt = 0;
}

void Enemy::ActionShot::Update(Enemy& _owner)
{
	const std::shared_ptr<Enemy_Gun>	spGun			= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Main>	spEnemy_Main	= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Player>		spPlayer		= _owner.m_wpPlayer.lock();

	Math::Vector3 targetPos;

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
	if (_owner.m_deathFlg)
	{
		// "ActionDeath"に切り替え
		_owner.ChangeActionState(std::make_shared<ActionDeath>());
		return;
	}
}

void Enemy::ActionReload::Enter(Enemy& _owner)
{
	// 敵の状態が"リロード状態"でないとき
	if (_owner.m_sitType != SituationType::Reload)
	{
		// 敵の状態を"リロード状態"に切り替え、
		// アニメーションを"Reload"に切り替え
		_owner.m_sitType = SituationType::Reload;
		_owner.ChangeAnimation("Reload", false);
	}

	// 現在のアニメーションフレームが0以外であれば
	if (_owner.m_nowAnimeFrm != _owner.m_zeroAnimFrm)
	{
		// 現在のアニメーションフレームを初期化
		_owner.m_nowAnimeFrm = _owner.m_zeroAnimFrm;
	}

}

void Enemy::ActionReload::Update(Enemy& _owner)
{
	const std::shared_ptr<Enemy_Main>			spMain				= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Enemy_Gun>			spGun				= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Magazine>		spMagazine			= _owner.m_wpEnemy_Magazine.lock();
	const std::shared_ptr<Enemy_Gun_NoMagazine>	spGun_NoMagazine	= _owner.m_wpEnemy_Gun_NoMagazine.lock();

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

			// ActionIdleに切り替え
			_owner.ChangeActionState(std::make_shared<ActionIdle>());
			return;
		}
	}
}

void Enemy::ActionDeath::Enter(Enemy& _owner)
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

void Enemy::ActionDeath::Update(Enemy& _owner)
{
	const std::shared_ptr<Enemy_Main>			spEnemy_Main			= _owner.m_wpEnemy_Main.lock();
	const std::shared_ptr<Enemy_Gun>			spEnemy_Gun				= _owner.m_wpEnemy_Gun.lock();
	const std::shared_ptr<Enemy_Gun_NoMagazine>	spEnemy_Gun_NoMagazine	= _owner.m_wpEnemy_Gun_NoMagazine.lock();
	const std::shared_ptr<Enemy_Magazine>		spEnemy_Magazine		= _owner.m_wpEnemy_Magazine.lock();


	// アニメーションフレームを更新
	_owner.m_nowAnimeFrm += _owner.m_animFrmSpd;

	// アニメーションが終了していれば
	if (_owner.m_nowAnimeFrm >= _owner.m_reloadAnimFrm)
	{
		_owner.m_dissolveFlg = true;
	}

	// ディゾルブフラグがtrueのとき
	if (_owner.m_dissolveFlg)
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
}
