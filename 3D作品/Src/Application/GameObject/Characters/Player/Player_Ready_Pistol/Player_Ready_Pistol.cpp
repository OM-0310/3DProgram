﻿#include "Player_Ready_Pistol.h"

#include "../Player.h"

#include "../../../Bullet/Bullet.h"

#include "../../../../Scene/SceneManager.h"

void Player_Ready_Pistol::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_Ready_Pistol/Player_Ready_Pistol.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos			= { 0.f,-0.9f,-50.0f };

	m_nowBullet		= m_magazineSize;

	m_shotWait		= m_shotWaitMin;

	m_rayBulletFlg	= false;
	m_shotFlg		= false;

	m_localMuzzleMat = Math::Matrix::CreateTranslation({ 0.0f,1.56f,1.0f });

	m_alpha = m_alphaMin;

	m_objectType = KdGameObject::ObjectType::TypePlayer;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player_Ready_PistolCollsion", m_spModel, KdCollider::TypeEvent | KdCollider::TypeBump);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player_Ready_Pistol::Update()
{
	// プレイヤー情報取得
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	Math::Matrix parentMat;

	// プレイヤー情報があるとき
	if (spPlayer)
	{
		// プレイヤーの行列を取得
		parentMat = spPlayer->GetMatrix();
	}

	// 銃口位置をデバッグ表示
	Math::Vector3 muzzlePos;
	muzzlePos = (m_localMuzzleMat * parentMat).Translation();

	//KdCollider::SphereInfo sphereInfo;
	//sphereInfo.m_sphere.Center = muzzlePos;
	//sphereInfo.m_sphere.Radius = 0.05f;
	//sphereInfo.m_type = KdCollider::TypeDamage;

	//m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kBlueColor);

	KdCollider::RayInfo rayInfo;
	rayInfo.m_pos = muzzlePos;
	rayInfo.m_dir = parentMat.Backward();
	rayInfo.m_range = 1000.f;
	rayInfo.m_type = KdCollider::TypeDamage | KdCollider::TypeGround;

	//m_pDebugWire->AddDebugLine(rayInfo.m_pos, rayInfo.m_dir, rayInfo.m_range, kBlueColor);

	std::list<KdCollider::CollisionResult> retRayList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(rayInfo, &retRayList);
	}


	bool hit = false;
	float maxOverLap = 0.f;
	Math::Vector3 hitPos = {};

	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			hit = true;
		}
	}

	if (m_nowBullet > m_magazineEmpty)
	{
		if (m_rayBulletFlg)
		{
			if (hit)
			{
				if (m_shotWait <= m_shotWaitMin)
				{
					m_nowBullet--;
					m_shotWait = m_shotWaitMax;

					// レイの着弾地点を利用して弾を飛ばすベクトルを算出
					Math::Vector3 bulletDir = hitPos - muzzlePos;

					// 発射
					std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
					bullet->Init();
					bullet->Shot(muzzlePos, bulletDir);
					SceneManager::Instance().AddObject(bullet);
				}
			}
		}
	}

	// プレイヤー情報があるとき
	if (spPlayer)
	{
		// ワールド行列にプレイヤーの行列を格納
		m_mWorld = spPlayer->GetMatrix();

		// プレイヤーの状態が"構え状態"または"リロード状態"または"弾発射状態"であるとき
		if (spPlayer->GetSituationType() & Player::SituationType::Ready				||
			spPlayer->GetSituationType() & Player::SituationType::Reload			||
			spPlayer->GetSituationType() & Player::SituationType::Restraint			||
			spPlayer->GetSituationType() & Player::SituationType::Restraint_Idle	||
			spPlayer->GetSituationType() & Player::SituationType::Kill				||
			spPlayer->GetSituationType() & Player::SituationType::Shot)
		{
			// アルファ値を最大値にする
			m_alpha = m_alphaMax;
		}
		// それ以外の状態のとき
		else
		{
			// アルファ値を最小値にする
			m_alpha = m_alphaMin;
		}
	}

	// 発射待機時間を減少する
	m_shotWait--;

	// 発射待機時間が最小値以下のとき
	if (m_shotWait < m_shotWaitMin)
	{
		// 発射待機時間を最小値に設定
		m_shotWait = m_shotWaitMin;
	}

	// 残弾数がマガジンサイズより大きいとき
	if (m_nowBullet >= m_magazineSize)
	{
		// 残弾数をマガジンサイズに設定
		m_nowBullet = m_magazineSize;
	}

	// 残弾数が0以下のとき
	if (m_nowBullet < m_magazineEmpty)
	{
		// 残弾数を0に設定
		m_nowBullet = m_magazineEmpty;
	}

	// 色情報の確定
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Player_Ready_Pistol::PostUpdate()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		m_spAnimator->AdvanceTime(m_spModel->WorkNodes(), spPlayer->GetAnimeSpeed());
	}
}

void Player_Ready_Pistol::DrawUnLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void Player_Ready_Pistol::ShotBullet(const bool rayFlg)
{
	// 発射フラグON
	m_shotFlg = true;

	// レイ = 弾とするか？
	m_rayBulletFlg = rayFlg;
}
