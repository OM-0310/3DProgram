﻿#include "EnemyBullet.h"

#include "../../Scene/SceneManager.h"

void EnemyBullet::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Bullet/Bullet.gltf");
		m_spModel = KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Bullet/Bullet.gltf");
	}

	m_tPoly.SetMaterial("Asset/Textures/Orbit/Orbit.png");
	m_tPoly.SetLength(25);

	m_mLocalTrail	= Math::Matrix::CreateTranslation(m_localTrailPos);

	m_pos			= {};
	m_moveDir		= {};

	m_lifeSpan		= m_lifeSpanMax;

	m_pDebugWire	= std::make_unique<KdDebugWireFrame>();
}

void EnemyBullet::Update()
{
	m_pos += m_moveDir * m_moveSpeed;

	m_lifeSpan--;
	if (m_lifeSpan < m_lifeSpanMin)
	{
		m_isExpired = true;
	}

	//=======================================================
	// 球判定・・・ここから
	//=======================================================

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Radius = m_hitArea;
	sphere.m_type = KdCollider::TypeDamage | KdCollider::TypeGround;

	// デバッグ確認用
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

	bool hit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// オブジェクトのタイプがプレイヤーのとき
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypePlayer)
		{
			// 当たり判定をする
			hit = obj->Intersects(sphere, nullptr);

			// 当たっているとき
			if (hit)
			{
				// 当たったオブジェクトのHPを減少させる
				obj->SetHPDec(BULLETPOW);

				// クラスを破棄する
				ClassDestruction();
				break;
			}
		}
		// オブジェクトのタイプが障害物のとき
		else if (obj->GetObjectType() == KdGameObject::ObjectType::TypeObstacles)
		{
			// 当たり判定をする
			hit = obj->Intersects(sphere, nullptr);

			// 当たっているとき
			if (hit)
			{
				// クラスを破棄する
				ClassDestruction();
				break;
			}
		}
	}

	//=======================================================
	// 球判定・・・ここまで
	//=======================================================

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mRot * m_mTrans;

	Math::Matrix trailTransMat = m_mWorld * m_mLocalTrail;

	// トレイルポリゴン頂点追加
	m_tPoly.AddPoint(trailTransMat);
}

void EnemyBullet::DrawLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(m_tPoly);
}

void EnemyBullet::Shot(const Math::Vector3 _pos, const Math::Vector3 _dir)
{
	// 弾の初期座標をコピー
	m_pos = _pos;

	// 進行ベクトルをコピー
	m_moveDir = _dir;
	m_moveDir.Normalize();

	//=====================================================================================================
	// 照準へのベクトルと弾の進行ベクトル
	//=====================================================================================================

	// ①ベクトルA　・・・　弾の現在の進行ベクトル
	Math::Vector3 _vecA = m_mWorld.Backward();
	_vecA.Normalize();

	// ②ベクトルB　・・・　照準への進行ベクトル
	Math::Vector3 _vecB = m_moveDir;
	_vecB.Normalize();

	// ①と②のベクトルの内積値から角度を算出
	float _dot = _vecA.Dot(_vecB);		// 内積値を算出
	float _angle = acos(_dot);			// 内積地から2つのベクトルの角度を算出

	// ①と②のベクトルの外積を利用して、回転する為のベクトルの角度を算出
	Math::Vector3 _rotAxis = _vecA.Cross(_vecB);

	// 内積で算出した角度文、外積で算出したベクトル(回転軸)で回転する行列を作成
	// = 弾がレティクルの方向を向く
	m_mRot = Math::Matrix::CreateFromAxisAngle(_rotAxis, _angle);
}
