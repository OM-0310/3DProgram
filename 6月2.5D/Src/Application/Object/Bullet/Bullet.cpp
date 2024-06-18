#include "Bullet.h"

#include "../Chara/Player/Player.h"
#include "../Chara/Enemy/Wild_Zombie/Wild_Zombie.h"
#include "../Chara/Enemy/Zombie_Woman/Zombie_Woman.h"

#include "../../Scene/GameScene/GameScene.h"
#include "../../Scene/SceneManager.h"

void Bullet::Update()
{
	m_pos += m_move;
	std::shared_ptr<Player>player = m_owner->GetPlayer();
	std::shared_ptr<Wild_Zombie>w_Zombie = m_owner->GetWild_Zombie();
	std::shared_ptr<Zombie_Woman>Zombie_w = m_owner->GetZombie_Woman();

	if (player->GetSizeX() == SIZE_NUM)m_sizeX = 0.05f;
	else if (player->GetSizeX() == -SIZE_NUM)m_sizeX = -0.05f;
	m_size.x = m_sizeX;

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Radius = m_hitArea;
	sphere.m_type = KdCollider::TypeDamage;

	// デバッグ確認用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

	// 球情報と当たり判定
	bool hit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypeEnemy)
		{
			hit = obj->Intersects(sphere, nullptr);

			// 攻撃が当たった場合
			if (hit)
			{
				if (obj->GetHP() != INT_ZERO)
				{
					obj->SetHPDec(1);
					// Hit時の自分の処理
					OnHit();

					break;
				}
			}
		}
	}
}

void Bullet::PostUpdate()
{
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mScale = Math::Matrix::CreateScale(m_size);
	m_mWorld = m_mScale * m_mTrans;
}

void Bullet::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Bullet/PlayerBullet.png");

	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Middle);

	m_objectType = KdGameObject::ObjectType::TypeBullet;

	m_pos = {};
	m_move = {};
	m_sizeX = 0.05f;
	m_size = { m_sizeX,0.05f,1.0f };
	m_hitArea = 0.1f;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Bullet::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Bullet::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
