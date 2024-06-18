#include "Squeal.h"

#include "../Chara/Player/Player.h"
#include "../../Scene/GameScene/GameScene.h"
#include "../../Scene/SceneManager.h"

void Squeal::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Scream/Scream.png");

	m_pos = {};
	m_size = { 2.0f,2.0f,2.0f };
	m_hitArea = 0.5f;
	m_count = INT_ZERO;

	Math::Matrix::Identity;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Squeal::Update()
{
	std::shared_ptr<Player> player = m_owner->GetPlayer();

	m_count++;
	if (m_count >= 30)
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
	sphereInfo.m_sphere.Radius = m_hitArea;

	// 当たり判定をしたいタイプを設定
	sphereInfo.m_type = KdCollider::TypeDamage;

	// デバッグ用
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kRedColor);

	// 球情報と当たり判定
	bool hit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypePlayer)
		{
			hit = obj->Intersects(sphereInfo, nullptr);

			// 攻撃が当たった場合
			if (hit)
			{
				if (obj->GetHP() != INT_ZERO)
				{
					player->UnableAct();


					break;
				}
			}
		}
	}

	//================================
	// 当たり判定　…　ここまで
	//================================
}

void Squeal::PostUpdate()
{
	m_mScale = Math::Matrix::CreateScale(m_size);
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mScale * m_mTrans;
}

void Squeal::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
