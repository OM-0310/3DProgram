#include "Zombie_Woman.h"

#include "../../../Squeal/Squeal.h"
#include "../../Player/Player.h"

#include "../../../../Scene/SceneManager.h"

Zombie_Woman::Zombie_Woman()
{
}

Zombie_Woman::~Zombie_Woman()
{
	Release();
}

void Zombie_Woman::Update()
{
	std::shared_ptr<Player> player = GetPlayer();

	if (m_aliveFlg)
	{
		switch (m_zwType)
		{
		case Zombie_WomanType::Idle:
			IdleUpdate();
			break;
		case Zombie_WomanType::Walk:
			WalkUpdate();
			break;
		case Zombie_WomanType::Scream:
			ScreamUpdate();
			break;
		case Zombie_WomanType::Hurt:
			HurtUpdate();
			break;
		case Zombie_WomanType::Dead:
			DeadUpdate();
			break;
		}

		m_color = kWhiteColor;
		//=======================================================================
		// 行動処理(ループ)　・・・　ここから
		//=======================================================================
		if (m_HP != INT_ZERO)
		{
			if (!m_followFlg)
			{
				m_actionCnt++;
				if (m_actionCnt < 60)
				{
					m_idleFlg = true;
				}
				if (m_actionCnt > 60 && m_actionCnt < 240)
				{
					m_idleFlg = false;
					m_sizeX = SIZE_NUM;
					m_move.x = 0.01f;
					m_walkFlg = true;
				}
				if (m_actionCnt > 240 && m_actionCnt < 360)
				{
					m_walkFlg = false;
					m_idleFlg = true;
				}
				if (m_actionCnt > 360 && m_actionCnt < 600)
				{
					m_idleFlg = false;
					m_sizeX = -SIZE_NUM;
					m_move.x = -0.01f;
					m_walkFlg = true;
				}
				if (m_actionCnt > 600)
				{
					m_walkFlg = false;
					m_actionCnt = 0;
				}
			}
		//=======================================================================
		// 行動処理(ループ)　・・・　ここまで
		//=======================================================================

			KdCollider::SphereInfo sphere1;
			sphere1.m_sphere.Center = m_pos + Math::Vector3(0, 0.15f, 0);
			sphere1.m_sphere.Radius = m_searchArea;

			KdCollider::SphereInfo sphere2;
			sphere2.m_sphere.Center = m_pos + Math::Vector3(0, 0.15f, 0);
			sphere2.m_sphere.Radius = m_screamArea;

			// デバッグ用
			m_pDebugWire->AddDebugSphere(sphere1.m_sphere.Center, sphere1.m_sphere.Radius, kGreenColor);
			m_pDebugWire->AddDebugSphere(sphere2.m_sphere.Center, sphere2.m_sphere.Radius, kBlueColor);

		//=======================================================================
		//一定範囲内に入ると追従　・・・　ここから
		//=======================================================================

			if (player->GetHP() != INT_ZERO)
			{
				Math::Vector3 vec;
				vec = player->GetPos() - m_pos;

				// 一定の範囲内に入ると追従する
				if (vec.Length() < m_searchArea)
				{
					if (!m_screamFlg)
					{
						m_followFlg = true;
						m_walkFlg = true;
						m_actionCnt = INT_ZERO;
						Follow();
					}
				}
				else
				{
					if (m_actionCnt == INT_ZERO)
					{
						m_followFlg = false;
						m_walkFlg = false;
					}
				}

				// 一定の範囲内に入ると奇声攻撃をする
				if (vec.Length() < m_screamArea)
				{
					m_screamActCnt++;
					m_actionCnt = INT_ZERO;
					m_followFlg = false;
					m_walkFlg = false;
					if (m_screamActCnt < 210)
					{
						m_idleFlg = true;
					}
					if (m_screamActCnt >= 210 && m_screamActCnt < 240)
					{
						if (m_screamWait == INT_ZERO)
						{
							m_screamWait = 120;
							m_idleFlg = false;
							m_screamFlg = true;
						}
					}
					if (m_screamActCnt >= 290)
					{
						m_screamActCnt = INT_ZERO;
						m_screamFlg = false;
					}

				}

			}
		//=======================================================================
		//一定範囲内に入ると追従　・・・　ここまで
		//=======================================================================
		}

		if (m_idleFlg)
		{
			m_zwType = Zombie_WomanType::Idle;
			m_move.x = FLOAT_ZERO;
		}
		else
		{
			m_idleAnim = FLOAT_ZERO;
		}
		if (m_walkFlg)
		{
			m_zwType = Zombie_WomanType::Walk;
			m_idleFlg = false;
			m_hurtFlg = false;
		}
		else
		{
			m_walkAnim = FLOAT_ZERO;
		}
		if (m_screamFlg)
		{
			m_zwType = Zombie_WomanType::Scream;
			m_idleFlg = false;
			m_walkFlg = false;
		}
		else
		{
			m_screamAnim = FLOAT_ZERO;
		}
		if (m_hurtFlg)
		{
			m_zwType = Zombie_WomanType::Hurt;
			m_idleFlg = false;
			m_walkFlg = false;
			m_screamFlg = false;
			m_move.x = FLOAT_ZERO;
			m_actionCnt = INT_ZERO;
		}
		else
		{
			m_hurtAnim = FLOAT_ZERO;
		}
		if (m_deadFlg)
		{
			m_zwType = Zombie_WomanType::Dead;
			m_idleFlg = false;
			m_walkFlg = false;
			m_screamFlg = false;
			m_hurtFlg = false;
			m_move.x = FLOAT_ZERO;
			m_actionCnt = INT_ZERO;
		}
		else
		{
			m_deadAnim = FLOAT_ZERO;
		}

		//=======================================================================
		// 奇声攻撃待機時間処理　・・・　ここから
		//=======================================================================
		m_screamWait--;
		if (m_screamWait <= INT_ZERO)
		{
			m_screamWait = INT_ZERO;
		}
		//=======================================================================
		// 奇声攻撃待機時間処理　・・・　ここまで
		//=======================================================================

		//=======================================================================
		// 無敵時間処理　・・・　ここから
		//=======================================================================
		m_invinWait--;
		if (m_invinWait <= INT_ZERO)
		{
			m_invinWait = INT_ZERO;
		}
		//=======================================================================
		// 無敵時間処理　・・・　ここまで
		//=======================================================================

		//=======================================================================
		// HP処理　・・・　ここから
		//=======================================================================

		// HPが0を下回ろうとしたら0に固定
		if (m_HP < INT_ZERO)
		{
			m_HP = INT_ZERO;
		}

		// HPが0になったら死亡時フラグをtrueにする
		if (m_HP == INT_ZERO)
		{
			m_deadFlg = true;
		}
		//=======================================================================
		// HP処理　・・・　ここまで
		//=======================================================================

		m_size.x = m_sizeX;
	}
}

void Zombie_Woman::PostUpdate()
{
	if (m_aliveFlg)
	{
		//=======================================================================
		// 重力処理　・・・　ここから
		//=======================================================================
		m_pos.y -= m_gravity;
		m_gravity += 0.005f;
		//=======================================================================
		// 重力処理　・・・　ここまで
		//=======================================================================

		// 座標を確定
		m_pos += m_move;

		//=======================================================================
		// 行列作成　・・・　ここから
		//=======================================================================
		m_mTrans = Math::Matrix::CreateTranslation(m_pos);
		m_mScale = Math::Matrix::CreateScale(m_size);
		m_mWorld = m_mScale * m_mTrans;
		//========================================================================
		// 行列作成　・・・　ここまで
		//========================================================================
		
		//========================================================================
		// 当たり判定(レイ判定)　・・・　ここから
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
		bool ishit1 = false;		// 当たっていたらtrue;
		for (auto& ret : retRayList)
		{
			if (maxOverLap < ret.m_overlapDistance)
			{
				maxOverLap = ret.m_overlapDistance;
				hitPos = ret.m_hitPos;
				ishit1 = true;
			}
		}
		if (ishit1)
		{
			// 当たっている
			m_pos = hitPos + Math::Vector3(0, -0.01f, 0);
			m_gravity = 0;
		}
		//========================================================================
		// 当たり判定(レイ判定)　・・・　ここまで
		//========================================================================
	}
}

//================================================================================
// 直立時更新　・・・　ここから
//================================================================================
void Zombie_Woman::IdleUpdate()
{
	m_idleAnim += 0.1f;
	if (m_idleAnim > IDLE_NUM)
	{
		m_idleAnim = FLOAT_ZERO;
	}
	m_spPoly->SetUVRect(m_idleCnt[(int)m_idleAnim]);
}
//================================================================================
// 直立時更新　・・・　ここまで
//================================================================================

//================================================================================
// 歩行時更新　・・・　ここから
//================================================================================
void Zombie_Woman::WalkUpdate()
{
	m_walkAnim += 0.1f;
	if (m_walkAnim > WALK_NUM)
	{
		m_walkAnim = FLOAT_ZERO;
	}
	m_spPoly->SetUVRect(m_walkCnt[(int)m_walkAnim]);
}
//================================================================================
// 歩行時更新　・・・　ここまで
//================================================================================

//================================================================================
// 奇声攻撃時更新　・・・　ここから
//================================================================================
void Zombie_Woman::ScreamUpdate()
{
	m_screamAnim += 0.1f;
	if (m_screamAnim > 1.0f)
	{
		Loudly();
	}
	if (m_screamAnim > SCREAM_NUM)
	{
		m_screamFlg = false;
		m_screamAnim = FLOAT_ZERO;
	}
	m_spPoly->SetUVRect(m_screamCnt[(int)m_screamAnim]);
}
//================================================================================
// 奇声攻撃時更新　・・・　ここまで
//================================================================================

//================================================================================
// 怯み時更新　・・・　ここから
//================================================================================
void Zombie_Woman::HurtUpdate()
{
	m_hurtAnim += 0.1f;
	if (m_hurtAnim > HURT_NUM)
	{
		m_hurtFlg = false;
		m_hurtAnim = FLOAT_ZERO;
		m_actionCnt = INT_ZERO;
		m_zwType = Zombie_WomanType::Idle;
	}
	m_spPoly->SetUVRect(m_hurtCnt[(int)m_hurtAnim]);
}
//================================================================================
// 怯み時更新　・・・　ここまで
//================================================================================

//================================================================================
// 死亡時更新　・・・　ここから
//================================================================================
void Zombie_Woman::DeadUpdate()
{
	m_deadAnim += 0.1f;
	if (m_deadAnim > DEAD_NUM)
	{
		m_deadAnim = DEAD_NUM - 1;
	}
	if (m_deadAnim == DEAD_NUM - 1)
	{
		m_dissolveCnt += 0.05f;
	}
	if (m_dissolveCnt > 1.0f)
	{
		m_dissolveCnt = 1.0f;
		m_aliveFlg = false;
	}
	m_spPoly->SetUVRect(m_deadCnt[(int)m_deadAnim]);
}
//================================================================================
// 死亡時更新　・・・　ここまで
//================================================================================

void Zombie_Woman::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dissolveCnt);

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld, m_color);
}

void Zombie_Woman::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Enemy/Zombie_Woman/Zombie_Woman.png");
	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	m_spPoly->SetSplit(9, 4);

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Zombie_WomanCollision", m_spPoly, KdCollider::TypeDamage);

	m_zwType		= Zombie_WomanType::Idle;
	m_objectType	= KdGameObject::ObjectType::TypeEnemy;
	m_pos			= { 0,1,0 };
	m_HP			= 7;

	m_idleAnim		= FLOAT_ZERO;
	m_walkAnim		= FLOAT_ZERO;
	m_screamAnim	= FLOAT_ZERO;
	m_hurtAnim		= FLOAT_ZERO;
	m_deadAnim		= FLOAT_ZERO;
	
	m_idleFlg		= false;
	m_walkFlg		= false;
	m_screamFlg		= false;
	m_hurtFlg		= false;
	m_deadFlg		= false;

	m_aliveFlg		= true;
	m_followFlg		= false;
	m_sizeX			= 0.8f;
	m_size			= { m_sizeX,0.8f,0.8f };
	m_color			= kWhiteColor;
	m_gravity		= FLOAT_ZERO;
	m_dissolveCnt	= FLOAT_ZERO;
	m_actionCnt		= INT_ZERO;
	m_screamWait	= INT_ZERO;
	m_screamActCnt	= INT_ZERO;
	m_searchArea	= 1.5f;
	m_screamArea	= 1.0f;
	m_invinWait		= INT_ZERO;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

//================================================================================
// 追従処理　・・・　ここから
//================================================================================
void Zombie_Woman::Follow()
{
	if (m_HP != INT_ZERO)
	{
		std::shared_ptr<Player> player = GetPlayer();
		if (m_pos.x > player->GetPos().x)
		{
			m_sizeX = -SIZE_NUM;
			m_move.x = -0.02f;
		}
		else if (m_pos.x < player->GetPos().x)
		{
			m_sizeX = SIZE_NUM;
			m_move.x = 0.02f;
		}
	}
}
//================================================================================
// 追従処理　・・・　ここまで
//================================================================================

//================================================================================
// 奇声攻撃処理　・・・　ここから
//================================================================================
void Zombie_Woman::Loudly()
{
	std::shared_ptr<Squeal> squeal;
	squeal = std::make_shared<Squeal>();
	squeal->Init();
	squeal->SetPos(m_pos);
	squeal->SetOwner(m_owner);
	SceneManager::Instance().AddObject(squeal);
}
//================================================================================
// 奇声攻撃処理　・・・　ここまで
//================================================================================

//================================================================================
// 怯み処理　・・・　ここから
//================================================================================
void Zombie_Woman::KnockBuck()
{
	std::shared_ptr<Player> player = GetPlayer();
	m_hurtFlg = true;
	if (m_hurtFlg)
	{
		if (m_invinWait == INT_ZERO)
		{
			m_invinWait = 10;
			SetHPDec(2);
			if (m_pos.x > player->GetPos().x)m_pos.x += 0.25f;
			else if (m_pos.x < player->GetPos().x)m_pos.x -= 0.25f;
		}
	}
}
//================================================================================
// 怯み処理　・・・　ここまで
//================================================================================

//================================================================================
// HP減少処理　・・・　ここから
//================================================================================
void Zombie_Woman::SetHPDec(int _decNum)
{
	if (m_invinWait == INT_ZERO)
	{
		m_invinWait = 10;
		m_color = kRedColor;
		m_HP -= _decNum;
	}
}
//================================================================================
// HP減少処理　・・・　ここまで
//================================================================================

std::shared_ptr<Player> Zombie_Woman::GetPlayer()
{
	
	if (m_wpPlayer.expired() == false)
	{
		return m_wpPlayer.lock();
	}
}

void Zombie_Woman::Release()
{
	m_spPoly = nullptr;
}
