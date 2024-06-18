#include "Wild_Zombie.h"
#include "../../Player/Player.h"
#include "../../../../Scene/SceneManager.h"

Wild_Zombie::Wild_Zombie()
{
}

Wild_Zombie::~Wild_Zombie()
{
	Release();
}

void Wild_Zombie::Update()
{
	std::shared_ptr<Player> player = GetPlayer();

	if (m_aliveFlg)
	{
		m_move = Math::Vector3::Zero;
		switch (m_wzType)
		{
		case Wild_Zombie::Idle:
			IdleUpdate();
			break;
		case Wild_Zombie::Walk:
			WalkUpdate();
			break;
		case Wild_Zombie::Run:
			RunUpdate();
			break;
		case Wild_Zombie::Jump:
			JumpUpdate();
			break;
		case Wild_Zombie::Atk:
			AtkUpdate();
			break;
		case Wild_Zombie::Hurt:
			HurtUpdate();
			break;
		case Wild_Zombie::Dead:
			DeadUpdate();
			break;
		case Wild_Zombie::Eat:
			EatUpdate();
			break;
		default:
			break;
		}

		m_color = kWhiteColor;

		//================================================================================
		// 行動処理(ループ)　・・・　ここから
		//================================================================================
		if (m_HP != INT_ZERO)
		{
			m_actionCnt++;
			if (m_actionCnt < 120)
			{
				m_idleFlg = true;
			}
			if (m_actionCnt > 120 && m_actionCnt < 360)
			{
				m_idleFlg = false;
				m_sizeX = SIZE_NUM;
				m_move.x = 0.01f;
				m_walkFlg = true;
			}
			if (m_actionCnt > 360 && m_actionCnt < 480)
			{
				m_walkFlg = false;
				m_idleFlg = true;
			}
			if (m_actionCnt > 480 && m_actionCnt < 720)
			{
				m_idleFlg = false;
				m_sizeX = -SIZE_NUM;
				m_move.x = -0.01f;
				m_walkFlg = true;
			}
			if (m_actionCnt > 720)
			{
				m_walkFlg = false;
				m_actionCnt = 0;
			}
		}
		//================================================================================
		// 行動処理(ループ)　・・・　ここまで
		//================================================================================

		KdCollider::SphereInfo sphere1;
		sphere1.m_sphere.Center = m_pos + Math::Vector3(0, 0.15f, 0);
		sphere1.m_sphere.Radius = m_searchArea;

		KdCollider::SphereInfo sphere2;
		sphere2.m_sphere.Center = m_pos + Math::Vector3(0, 0.15f, 0);
		sphere2.m_sphere.Radius = m_attackArea;

		// デバッグ用
		m_pDebugWire->AddDebugSphere(sphere1.m_sphere.Center, sphere1.m_sphere.Radius, kGreenColor);
		m_pDebugWire->AddDebugSphere(sphere2.m_sphere.Center, sphere2.m_sphere.Radius, kBlueColor);

		//================================================================================
		// 一定範囲内に入ると追従　・・・　ここから
		//================================================================================
		if (player->GetHP() != INT_ZERO)
		{
			Math::Vector3 vec;
			vec = player->GetPos() - m_pos;
			if (vec.Length() < m_searchArea)
			{
				m_runFlg = true;
			}
			else
			{
				m_runFlg = false;
			}
			if (vec.Length() < m_attackArea)
			{
				m_runFlg = false;
				m_atkFlg = true;
				if (m_atkFlg)
				{
					Attack(m_atkFlg);
				}
			}
		}
		//================================================================================
		// 一定範囲内に入ると追従　・・・　ここまで
		//================================================================================

		//================================================================================
		// 無敵時間処理　・・・　ここから
		//================================================================================
		m_invinWait--;
		if (m_invinWait <= INT_ZERO)
		{
			m_invinWait = INT_ZERO;
		}
		//================================================================================
		// 無敵時間処理　・・・　ここまで
		//================================================================================

		//================================================================================
		// HP処理　・・・　ここから
		//================================================================================
		if (m_HP < INT_ZERO)
		{
			m_HP = INT_ZERO;
		}
		if (m_HP == INT_ZERO)
		{
			m_deadFlg = true;
		}
		//================================================================================
		// HP処理　・・・　ここまで
		//================================================================================

		//================================================================================
		// プレイヤーのHPが0になったら捕食状態になる　・・・　ここから
		//================================================================================
		if (player->GetHP() == INT_ZERO)
		{
			Math::Vector3 vec;
			vec = player->GetPos() - m_pos;
			if (vec.Length() < 0.3f)
			{
				m_eatFlg = true;
			}
		}
		//================================================================================
		// プレイヤーのHPが0になったら捕食状態になる　・・・　ここまで
		//================================================================================

		if (m_idleFlg)
		{
			m_wzType = Wild_ZombieType::Idle;
			m_move.x = FLOAT_ZERO;
		}
		if (m_walkFlg)
		{
			m_wzType = Wild_ZombieType::Walk;
			m_idleFlg = false;
			m_runFlg = false;
			m_atkFlg = false;
			m_hurtFlg = false;
		}
		if (m_runFlg)
		{
			m_wzType = Wild_ZombieType::Run;
			Follow();
			m_idleFlg = false;
			m_walkFlg = false;
			m_actionCnt = INT_ZERO;
		}
		if (!m_hurtFlg)
		{
			if (m_atkFlg)
			{
				m_wzType = Wild_ZombieType::Atk;
				m_idleFlg = false;
				m_walkFlg = false;
				m_runFlg = false;
				m_actionCnt = INT_ZERO;
				m_move.x = FLOAT_ZERO;
				if (m_pos.x > m_wpPlayer.lock()->GetPos().x)		m_sizeX = -SIZE_NUM;
				else if (m_pos.x < m_wpPlayer.lock()->GetPos().x)	m_sizeX = SIZE_NUM;
			}
		}
		if (m_hurtFlg)
		{
			m_wzType = Wild_ZombieType::Hurt;
			m_atkFlg = false;
			m_idleFlg = false;
			m_walkFlg = false;
			m_runFlg = false;
			m_actionCnt = INT_ZERO;
			m_move.x = FLOAT_ZERO;
		}
		if (m_deadFlg)
		{
			m_wzType = Wild_ZombieType::Dead;
			m_atkFlg = false;
			m_idleFlg = false;
			m_walkFlg = false;
			m_runFlg = false;
			m_hurtFlg = false;
			m_actionCnt = INT_ZERO;
			m_move.x = FLOAT_ZERO;
		}
		if (m_eatFlg)
		{
			m_wzType = Wild_ZombieType::Eat;
			m_atkFlg = false;
			m_idleFlg = false;
			m_walkFlg = false;
			m_runFlg = false;
			m_hurtFlg = false;
			m_actionCnt = INT_ZERO;
			m_move.x = FLOAT_ZERO;
		}
	}
}

void Wild_Zombie::PostUpdate()
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
		m_mScale = Math::Matrix::CreateScale(m_sizeX, SIZE_NUM, SIZE_NUM);
		m_mWorld = m_mScale * m_mTrans;
		//=======================================================================
		// 行列作成　・・・　ここまで
		//=======================================================================

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
void Wild_Zombie::IdleUpdate()
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
void Wild_Zombie::WalkUpdate()
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
// 走行時更新　・・・　ここから
//================================================================================
void Wild_Zombie::RunUpdate()
{
	m_runAnim += 0.1f;
	if (m_runAnim > RUN_NUM)
	{
		m_runAnim = FLOAT_ZERO;
		m_actionCnt = INT_ZERO;
	}
	m_spPoly->SetUVRect(m_runCnt[(int)m_runAnim]);
}
//================================================================================
// 走行時更新　・・・　ここまで
//================================================================================

void Wild_Zombie::JumpUpdate()
{
}

//================================================================================
// 攻撃時更新　・・・　ここから
//================================================================================
void Wild_Zombie::AtkUpdate()
{
	m_atkAnim += 0.1f;
	if (m_atkAnim > ATK_NUM)
	{
		m_atkFlg = false;
		m_actionCnt = INT_ZERO;
		m_atkAnim = FLOAT_ZERO;
		m_wzType = Wild_ZombieType::Idle;
	}
	m_spPoly->SetUVRect(m_atkCnt[(int)m_atkAnim]);
}
//================================================================================
// 攻撃時更新　・・・　ここまで
//================================================================================

//================================================================================
// 怯み時更新　・・・　ここから
//================================================================================
void Wild_Zombie::HurtUpdate()
{
	m_hurtAnim += 0.1f;
	if (m_hurtAnim > HURT_NUM)
	{
		m_hurtFlg = false;
		m_hurtAnim = INT_ZERO;
		m_actionCnt = INT_ZERO;
		m_wzType = Wild_ZombieType::Idle;
	}
	m_spPoly->SetUVRect(m_hurtCnt[(int)m_hurtAnim]);
}
//================================================================================
// 怯み時更新　・・・　ここまで
//================================================================================

//================================================================================
// 死亡時更新　・・・　ここから
//================================================================================
void Wild_Zombie::DeadUpdate()
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

//================================================================================
// 捕食時更新　・・・　ここから
//================================================================================
void Wild_Zombie::EatUpdate()
{
	m_eatAnim += 0.1f;
	if (m_eatAnim > EAT_NUM)
	{
		m_eatAnim = FLOAT_ZERO;
	}
	m_spPoly->SetUVRect(m_eatCnt[(int)m_eatAnim]);
}
//================================================================================
// 捕食時更新　・・・　ここまで
//================================================================================

void Wild_Zombie::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.SetDissolve(m_dissolveCnt);

	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld, m_color);
}

void Wild_Zombie::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Enemy/Wild_Zombie/Wild_Zombie.png");

	m_spPoly->SetSplit(12, 6);
	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_color = kWhiteColor;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Wild_ZombieCollision", m_spPoly, KdCollider::TypeDamage);

	m_wzType =		Wild_ZombieType::Idle;
	m_objectType =	KdGameObject::ObjectType::TypeEnemy;

	m_idleAnim	=	FLOAT_ZERO;
	m_walkAnim	=	FLOAT_ZERO;
	m_runAnim	=	FLOAT_ZERO;
	m_jumpAnim	=	FLOAT_ZERO;
	m_atkAnim	=	FLOAT_ZERO;
	m_hurtAnim	=	FLOAT_ZERO;
	m_deadAnim	=	FLOAT_ZERO;
	m_eatAnim	=	FLOAT_ZERO;

	m_sizeX		=	SIZE_NUM;
	m_gravity	=	FLOAT_ZERO;
	m_actionCnt =	INT_ZERO;
	m_dissolveCnt = FLOAT_ZERO;
	m_searchArea  = 1.5f;
	m_attackArea  = 0.4f;

	m_idleFlg	=	true;
	m_walkFlg	=	false;
	m_atkFlg	=	false;
	m_runFlg	=	false;
	m_hurtFlg	=	false;
	m_deadFlg	=	false;
	m_eatFlg	=	false;

	m_aliveFlg	=	true;
	m_pos		=	{ 2,1,0 };
	m_HP		=	5;
	m_invinWait =	INT_ZERO;

	// デバッグ用
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

//================================================================================
// 追従処理　・・・　ここから
//================================================================================
void Wild_Zombie::Follow()
{
	std::shared_ptr<Player> player = GetPlayer();
	if (m_runFlg)
	{
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
// 攻撃処理　・・・　ここから
//================================================================================
void Wild_Zombie::Attack(bool _hit)
{
	std::shared_ptr<Player> player = GetPlayer();
	if (_hit)
	{
		if ((int)m_atkAnim == 2 ||
			(int)m_atkAnim == 6 ||
			(int)m_atkAnim == 10)
		{
			player->SetHPDec(1);
		}
	}
}
//================================================================================
// 攻撃処理　・・・　ここまで
//================================================================================

//================================================================================
// 怯み処理　・・・　ここから
//================================================================================
void Wild_Zombie::KnockBuck()
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
void Wild_Zombie::SetHPDec(int _decNum)
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

std::shared_ptr<Player> Wild_Zombie::GetPlayer()
{
	std::shared_ptr<Player> player;
	if (!m_wpPlayer.expired())
	{
		player = m_wpPlayer.lock();
	}
	return player;
}

void Wild_Zombie::Release()
{
	m_spPoly = nullptr;
}
