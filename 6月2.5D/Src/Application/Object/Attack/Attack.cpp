#include "Attack.h"

#include "../Chara/Player/Player.h"
#include "../../Scene/GameScene/GameScene.h"
#include "../../Scene/SceneManager.h"

void Attack::Init()
{
	m_pos = { FLOAT_ZERO,-5.0f,FLOAT_ZERO };

	m_attackArea = 0.15f;
	m_attackCnt = INT_ZERO;
	m_attackCntMax = 10;

	m_mTrans = Math::Matrix::Identity;
	m_mWorld = Math::Matrix::Identity;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Attack::Update()
{
	std::shared_ptr<Player> player = m_owner->GetPlayer();
	m_attackCnt++;
	if (m_attackCnt > m_attackCntMax)
	{
		m_isExpired = true;
	}

	//================================
	// 当たり判定　…　ここから
	//================================

	// 球判定用の変数作成
	KdCollider::SphereInfo sphereInfo;

	// 球の中心位置を設定
	sphereInfo.m_sphere.Center = m_pos + Math::Vector3{ FLOAT_ZERO,0.45f,FLOAT_ZERO };

	// 球の半径を設定
	sphereInfo.m_sphere.Radius = m_attackArea;

	// 当たり判定をしたいタイプを設定
	sphereInfo.m_type = KdCollider::TypeDamage;

	// デバッグ用
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kRedColor);

	// 球情報と当たり判定
	bool hit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypeEnemy)
		{
			hit = obj->Intersects(sphereInfo, nullptr);

			// 攻撃が当たった場合
			if (hit)
			{
				if (obj->GetHP() != INT_ZERO)
				{
					obj->KnockBuck();
					// Hit時の自分の処理
					OnHit();

					break;
				}
			}
		}
	}

	//================================
	// 当たり判定　…　ここまで
	//================================

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;
}
