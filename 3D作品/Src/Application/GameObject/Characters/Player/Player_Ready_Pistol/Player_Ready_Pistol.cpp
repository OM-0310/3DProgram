#include "Player_Ready_Pistol.h"

#include "../Player.h"

#include "../../../Bullet/Bullet.h"

#include "../../../../Scene/SceneManager.h"

void Player_Ready_Pistol::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_Ready_Pistol.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos			= { 0.f,-0.9f,-50.0f };

	m_bulletNum		= m_magazineSize;

	m_shotWait		= 0.0f;

	m_shotFlg		= false;
	m_rayBulletFlg	= false;

	m_localMuzzleMat = Math::Matrix::CreateTranslation({-0.3f,1.5f,1.8f});

	m_alpha = m_alphaMin;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player_Ready_Pistol::Update()
{
	// プレイヤー情報取得
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

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

	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = muzzlePos;
	sphereInfo.m_sphere.Radius = 0.05f;
	sphereInfo.m_type = KdCollider::TypeDamage;

	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kBlueColor);

	m_shotWait -= m_time;
	if (m_shotWait <= 0.0f)
	{
		m_shotWait = 0.0f;
	}

	// 弾発射
	if (m_shotFlg)
	{
		KdCollider::RayInfo rayInfo;
		rayInfo.m_pos = muzzlePos;
		rayInfo.m_dir = parentMat.Backward();
		rayInfo.m_range = 100.f;
		rayInfo.m_type = KdCollider::TypeDamage | KdCollider::TypeGround;

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

		if (m_rayBulletFlg)
		{
			if (hit)
			{
				if (m_shotWait <= 0.0f)
				{
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
		// プレイヤーの行列を取得
		parentMat = spPlayer->GetMatrix();

		// ワールド行列にプレイヤーの行列を格納
		m_mWorld = spPlayer->GetMatrix();

		// プレイヤーの状態が"構え状態"または"リロード状態"または"弾発射状態"であるとき
		if (spPlayer->GetSituationType() & Player::SituationType::Ready ||
			spPlayer->GetSituationType() & Player::SituationType::Reload||
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

	// 色情報の確定
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void Player_Ready_Pistol::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
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
