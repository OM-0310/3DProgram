#include "Grenade.h"

#include "../Chara/Player/Player.h"
#include "../Chara/Enemy/Wild_Zombie/Wild_Zombie.h"

#include "../../Scene/SceneManager.h"
#include "../../Scene/GameScene/GameScene.h"

void Grenade::Update()
{
	std::shared_ptr<Player> player = m_owner->GetPlayer();
	std::shared_ptr<Wild_Zombie>w_Zombie = m_owner->GetWild_Zombie();
	if(m_throwFlg)m_move.y = m_throwPow;
	if (player->GetSizeX() == SIZE_NUM)
	{
		m_sizeX = SIZE_NUM;
		m_size = { m_sizeX,m_sizeY,SIZE_NUM };
		if (m_throwFlg)
		{
			m_move.x = 0.05f;
		}
	}
	else if (player->GetSizeX() == -SIZE_NUM)
	{
		m_sizeX = -SIZE_NUM;
		m_size = { m_sizeX,m_sizeY,SIZE_NUM };
		if (m_throwFlg)
		{
			m_move.x = -0.05f;
		}
	}

	if (m_animFlg)
	{
		m_animCnt += m_animSpeed;
	}
	if (m_animCnt >= 4.0f)
	{
		if (player->GetSizeX() == SIZE_NUM)
		{
			m_sizeX = 2.0f;
			m_sizeY = 2.0f;
			m_size = { m_sizeX,m_sizeY,SIZE_NUM };
		}
		else if (player->GetSizeX() == -SIZE_NUM)
		{
			m_sizeX = -2.0f;
			m_sizeY = 2.0f;
			m_size = { m_sizeX,m_sizeY,SIZE_NUM };
		}
	}
	if (m_animCnt >= EXP_NUM - 1)
	{
		m_animFlg = false;
		m_animCnt = FLOAT_ZERO;
		Explosion();
	}
	m_spPoly->SetUVRect(m_anime[(int)m_animCnt]);

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Radius = m_expArea;

	// デバッグ確認用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kRedColor);

	for (auto& obj : m_owner->GetObjList())
	{
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypeEnemy)
		{
			if (w_Zombie->GetHP() != INT_ZERO)
			{
				Math::Vector3 vec;
				vec = obj->GetPos() - m_pos;

				if (vec.Length() < m_expArea)
				{
					if (m_animCnt > 5.0f)
					{
						w_Zombie->SetHPDec(5);
						break;
					}
				}
			}
		}
	}
}

void Grenade::PostUpdate()
{
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;
	m_pos += m_move;
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mScale = Math::Matrix::CreateScale(m_size);
	m_mWorld = m_mScale * m_mTrans;
	//========================================================================
	// 当たり判定(レイ判定)
	//========================================================================

	KdCollider::RayInfo ray;
	ray.m_pos = m_pos;
	ray.m_dir = Math::Vector3::Down;
	ray.m_pos.y += 0.01f;

	float enebleStepHigh = 0.2f;
	ray.m_pos.y += enebleStepHigh;

	ray.m_range = m_gravity + enebleStepHigh;
	ray.m_type = KdCollider::TypeGround;

	// レイに当たったオブジェクト情報
	std::list<KdCollider::CollisionResult> retRayList;

	// レイと当たり判定
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		obj->Intersects(ray, &retRayList);
	}

	// デバッグ(レイを可視化)
	m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range);

	// レイリストから一番近いオブジェクトを検出
	float maxOverLap = 0;	// はみ出たレイの長さ
	Math::Vector3 hitPos;	// レイが遮断された座標(当たった座標)
	bool ishit = false;		// 当たっていたらtrue;
	for (auto& ret : retRayList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitPos = ret.m_hitPos;
			ishit = true;
		}
	}
	if (ishit)
	{
		// 当たっている
		m_pos = hitPos + Math::Vector3(0, -0.01f, 0);
		m_move = Math::Vector3::Zero;
		m_animFlg = true;
		m_throwFlg = false;
		m_gravity = 0;
	}
}

void Grenade::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Grenade/Explosion.png");
	m_spPoly->SetSplit(9, 1);

	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);


	m_pos = {};
	m_move = {};
	m_gravity = FLOAT_ZERO;

	m_throwFlg = true;
	m_animFlg = false;
	m_animCnt = FLOAT_ZERO;
	m_animSpeed = 0.25f;
	m_expArea = 1.0f;

	m_sizeX = SIZE_NUM;
	m_sizeY = SIZE_NUM;
	m_size = { m_sizeX,m_sizeY,1.0f };

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Grenade::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void Grenade::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
