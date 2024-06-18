#include "Player.h"

#include "../Enemy/Wild_Zombie/Wild_Zombie.h"
#include "../Enemy/Zombie_Woman/Zombie_Woman.h"
#include "../../Attack/Attack.h"
#include "../../Grenade/Grenade.h"
#include "../../Bullet/Bullet.h"

#include "../../../Scene/SceneManager.h"
#include "../../../Scene/GameScene/GameScene.h"

Player::Player()
{
}

Player::~Player()
{
	Release();
}

void Player::Update()
{
	if (m_aliveFlg)
	{
		switch (m_pType)
		{
		case PlayerType::Idle:
			IdleUpdate();
			break;
		case PlayerType::Walk:
			WalkUpdate();
			break;
		case PlayerType::Run:
			RunUpdate();
			break;
		case PlayerType::Atk:
			AtkUpdate();
			break;
		case PlayerType::Shot:
			ShotUpdate();
			break;
		case PlayerType::Throw:
			ThrowUpdate();
			break;
		case PlayerType::Hurt:
			HurtUpdate();
			break;
		case PlayerType::Dead:
			DeadUpdate();
			break;
		case PlayerType::Reload:
			ReloadUpdate();
			break;
		}


		if (m_HP != INT_ZERO)
		{
			if (m_statysType == StatusType::Normal)
			{
				m_color = kWhiteColor;

				// ==歩行処理=========================================================
				m_move = Math::Vector3::Zero;
				if (!m_setFlg)
				{
					if (m_pType != PlayerType::Atk &&
						m_pType != PlayerType::Reload &&
						m_pType != PlayerType::Throw &&
						m_pType != PlayerType::Hurt &&
						m_pType != PlayerType::Dead)
					{
						if (GetAsyncKeyState('D') & 0x8000)
						{
							m_sizeX = SIZE_NUM;
							m_move.x += 0.02f;
							m_pType = PlayerType::Walk;
							// ==走行処理==========================================
							if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
							{
								m_move.x += 0.05f;
								m_pType = PlayerType::Run;
							}
							else
							{
								m_runAnim = FLOAT_ZERO;
								m_pType = PlayerType::Walk;
							}
						}
						else if (GetAsyncKeyState('A') & 0x8000)
						{
							m_sizeX = -SIZE_NUM;
							m_move.x += -0.02f;
							m_pType = PlayerType::Walk;
							// ==走行処理==========================================
							if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
							{
								m_move.x += -0.05f;
								m_pType = PlayerType::Run;
							}
							else
							{
								m_runAnim = FLOAT_ZERO;
								m_pType = PlayerType::Walk;
							}
						}
						else
						{
							m_walkAnim = FLOAT_ZERO;
							m_pType = PlayerType::Idle;
						}
					}
				}

				// ==近接攻撃処理=====================================================
				if (!m_setFlg)
				{
					if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					{
						if (!m_keyFlg)
						{
							m_keyFlg = true;
							m_atkFlg = true;
						}
					}
					else
					{
						m_keyFlg = false;
					}
				}
				if (m_atkFlg)
				{
					m_pType = PlayerType::Atk;
				}

				// ==銃発射処理=========================================================
				if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
				{
					m_pType = PlayerType::Shot;
					if (!m_setFlg)
					{
						m_move = Math::Vector3::Zero;
						m_setFlg = true;
					}
				}
				else
				{
					m_shotAnim = FLOAT_ZERO;
					m_shotFlg = false;
					m_setFlg = false;
				}

				if (GetAsyncKeyState('G') & 0x8000)
				{
					if (m_grenadeNum > INT_ZERO)
					{
						if (!m_greFlg)
						{
							m_greFlg = true;
						}
					}
				}
				if (m_greFlg)
				{
					m_pType = PlayerType::Throw;
				}

				// ==リロード処理========================================================
				if (GetAsyncKeyState('R') & 0x8000)
				{
					if (m_bulletNum != BULLET_MAX)
					{
						m_relFlg = true;
					}
				}
				if (m_bulletNum == INT_ZERO)
				{
					m_relFlg = true;
				}

				if (m_relFlg)
				{
					m_pType = PlayerType::Reload;
				}

				if (m_hurtFlg)
				{
					m_pType = PlayerType::Hurt;
				}

				// デバッグ確認用
				if (GetAsyncKeyState('1') & 0x8000)
				{
					SetHPDec(0);
				}
				if (GetAsyncKeyState('2') & 0x8000)
				{
					Death();
				}
			}
		}
	}

	m_apraxiaWait--;
	if (m_apraxiaWait <= INT_ZERO)
	{
		m_apraxiaWait = INT_ZERO;
	}
	if (m_apraxiaWait == INT_ZERO)
	{
		m_statysType = StatusType::Normal;
	}
	if (m_statysType == StatusType::Apraxia)
	{
		m_color = kBlueColor;
		m_move.x = FLOAT_ZERO;
	}

	m_shotWait--;
	if (m_shotWait <= INT_ZERO)
	{
		m_shotWait = INT_ZERO;
	}
	m_throwWait--;
	if (m_throwWait <= INT_ZERO)
	{
		m_throwWait = INT_ZERO;
	}
	m_invinWait--;
	if (m_invinWait <= INT_ZERO)
	{
		m_invinWait = INT_ZERO;
	}
	if (m_bulletNum <= INT_ZERO)
	{
		m_bulletNum = INT_ZERO;
	}
	if (m_bulletNum >= BULLET_MAX)
	{
		m_bulletNum = BULLET_MAX;
	}
	if (m_grenadeNum <= INT_ZERO)
	{
		m_grenadeNum = INT_ZERO;
	}
	if (m_grenadeNum >= GRENADE_MAX)
	{
		m_grenadeNum = GRENADE_MAX;
	}
	if (m_HP < INT_ZERO)
	{
		m_HP = INT_ZERO;
	}
	if (m_HP == INT_ZERO)
	{
		m_deadFlg = true;
	}
	if (m_deadFlg)
	{
		m_hurtFlg = false;
		m_pType = PlayerType::Dead;
	}
}

void Player::PostUpdate()
{
	// 重力をキャラに反映
	m_pos.y -= m_gravity;
	m_gravity += 0.005f;
	m_pos += m_move;
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mScale = Math::Matrix::CreateScale(m_sizeX, SIZE_NUM, SIZE_NUM);
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
		m_gravity = 0;
	}

	//==================================================================
	//球判定
	//==================================================================
	// 球判定用の変数
	KdCollider::SphereInfo sphere;
	// 球の中心位置を設定
	sphere.m_sphere.Center = m_pos + Math::Vector3(0, 0.2f, 0);
	// 球の半径を設定
	sphere.m_sphere.Radius = 0.3f;
	// 当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround | KdCollider::TypeBump;

	// デバッグ用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

	// 弾が当たったオブジェクトの情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	// 球と当たり判定
	//for (auto& obj : SceneManager::Instance().GetObjList())
	//{
	//	if (obj->Intersects(sphere, &retSphereList))
	//	{
	//		// このif文に入ったら当たっている
	//		if (obj->GetObjectType() == KdGameObject::Enemy)
	//		{
	//			// このif文に入ったら敵確定
	//			obj->OnHit();
	//		}
	//	}
	//}

	// 球リストから一番近いオブジェクトを検出
	maxOverLap = 0;	// はみ出た球の長さ
	ishit = false;	// 当たっていたらtrue;
	Math::Vector3 hitDir;
	for (auto& ret : retSphereList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap = ret.m_overlapDistance;
			hitDir = ret.m_hitDir;
			ishit = true;
		}
	}
	if (ishit)
	{
		// Z方向無効
		hitDir.z = 0;

		// 方向ベクトルは長さ1にする必要がある
		hitDir.Normalize();

		// 押し戻し
		m_pos += hitDir * maxOverLap;
	}
}

void Player::IdleUpdate()
{
	m_idleAnim += 0.1f;
	if (m_idleAnim > IDLE_NUM)
	{
		m_idleAnim = FLOAT_ZERO;
	}
	m_spPoly->SetUVRect(m_idleCnt[(int)m_idleAnim]);
}

void Player::WalkUpdate()
{
	m_walkAnim += 0.1f;
	if (m_walkAnim > WALK_NUM)
	{
		m_walkAnim = FLOAT_ZERO;
	}
	m_spPoly->SetUVRect(m_walkCnt[(int)m_walkAnim]);
}

void Player::RunUpdate()
{
	m_runAnim += 0.15f;
	if (m_runAnim > RUN_NUM)
	{
		m_runAnim = FLOAT_ZERO;
	}
	m_spPoly->SetUVRect(m_runCnt[(int)m_runAnim]);
}

void Player::AtkUpdate()
{
	m_atkAnim += 0.15f;
	if (m_atkAnim > 1.0f)
	{
		Melee();
	}
	if (m_atkAnim > ATK_NUM)
	{
		m_atkAnim = FLOAT_ZERO;
		m_atkFlg = false;
		m_pType = PlayerType::Idle;
	}
	m_spPoly->SetUVRect(m_atkCnt[(int)m_atkAnim]);
}

void Player::ShotUpdate()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_shotFlg)
		{
			m_shotFlg = true;
		}
	}
	if (m_shotFlg)
	{
		m_shotAnim += 0.4f;
	}
	if (m_shotAnim > 2.0f)
	{
		if (m_shotWait == INT_ZERO)
		{
			GunShot();
			m_bulletNum--;
			m_shotWait = 10;
		}
	}
	if (m_shotAnim > SHOT_NUM)
	{
		m_shotAnim = FLOAT_ZERO;
		m_shotFlg = false;
	}
	m_spPoly->SetUVRect(m_shotCnt[(int)m_shotAnim]);
}

void Player::ThrowUpdate()
{
	m_throwAnim += 0.1f;
	if (m_throwAnim > 7.0f)
	{
		if (m_throwWait == INT_ZERO)
		{
			GreThrow();
			m_throwWait = 60;
		}
	}
	if (m_throwAnim > THROW_NUM)
	{
		m_throwAnim = FLOAT_ZERO;
		m_pType = PlayerType::Idle;
		m_greFlg = false;
	}
	m_spPoly->SetUVRect(m_throwCnt[(int)m_throwAnim]);
}

void Player::HurtUpdate()
{
	m_hurtAnim += 0.2f;
	if (m_hurtAnim > HURT_NUM)
	{
		m_hurtAnim = FLOAT_ZERO;
		m_hurtFlg = false;
		m_pType = PlayerType::Idle;
	}
	m_spPoly->SetUVRect(m_hurtCnt[(int)m_hurtAnim]);
}

void Player::DeadUpdate()
{
	m_deadAnim += 0.1f;
	if (m_deadAnim > DEAD_NUM)
	{
		m_deadAnim = DEAD_NUM - 1;
	}
	m_spPoly->SetUVRect(m_deadCnt[(int)m_deadAnim]);
}

void Player::ReloadUpdate()
{
	m_reloadAnim += 0.15f;
	if (m_reloadAnim > RELOAD_NUM)
	{
		m_reloadAnim = FLOAT_ZERO;
		m_bulletNum += 30;
		m_relFlg = false;
		m_pType = PlayerType::Idle;
	}
	m_spPoly->SetUVRect(m_reloadCnt[(int)m_reloadAnim]);
}

void Player::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Player/Player.png");

	m_spPoly->SetSplit(14, 5);

	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	m_HP = 10;
	m_pos = { -1,1,0 };
	m_move = Math::Vector3::Zero;
	m_color = kWhiteColor;

	m_idleAnim	=	FLOAT_ZERO;
	m_walkAnim	=	FLOAT_ZERO;
	m_runAnim	=	FLOAT_ZERO;
	m_atkAnim	=	FLOAT_ZERO;
	m_shotAnim	=	FLOAT_ZERO;
	m_throwAnim	=	FLOAT_ZERO;
	m_hurtAnim	=	FLOAT_ZERO;
	m_deadAnim	=	FLOAT_ZERO;
	m_reloadAnim =	FLOAT_ZERO;

	m_atkFlg	=	false;
	m_shotFlg	=	false;
	m_greFlg	=	false;
	m_relFlg	=	false;
	m_hurtFlg	=	false;
	m_deadFlg	=	false;
	m_keyFlg	=	false;

	m_sizeX		=	SIZE_NUM;
	m_gravity	=	FLOAT_ZERO;
	m_shotWait	=	INT_ZERO;
	m_throwWait =	INT_ZERO;
	m_invinWait =	INT_ZERO;
	m_apraxiaWait = INT_ZERO;

	m_pType			= PlayerType::Idle;
	m_statysType	= StatusType::Normal;
	m_objectType	= KdGameObject::ObjectType::TypePlayer;
	m_aliveFlg		= true;

	m_bulletNum		= 30;
	m_grenadeNum	= 2;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("PlayerCollision", m_spPoly, KdCollider::TypeSight | KdCollider::TypeDamage);

	// デバッグ用
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::GunShot()
{
	std::shared_ptr<Bullet> bullet;
	bullet = std::make_shared<Bullet>();
	bullet->Init();
	if (m_sizeX == SIZE_NUM)
	{
		bullet->SetPos(m_pos + Math::Vector3{ 0.2f,0.395f,0.0f });
		bullet->SetMove({ 0.1f,0.0f,0.0f });
	}
	else if (m_sizeX == -SIZE_NUM)
	{
		bullet->SetPos(m_pos + Math::Vector3{ -0.2f,0.395f,0.0f });
		bullet->SetMove({ -0.1f,0.0f,0.0f });
	}
	bullet->SetOwner(m_owner);
	SceneManager::Instance().AddObject(bullet);
	m_wpBullet = bullet;
}

void Player::GreThrow()
{
	std::shared_ptr<Grenade> grenade;
	grenade = std::make_shared<Grenade>();
	grenade->Init();
	grenade->SetPos(m_pos);
	grenade->SetOwner(m_owner);
	SceneManager::Instance().AddObject(grenade);
}

void Player::Melee()
{
	std::shared_ptr<Attack> attack;
	attack = std::make_shared<Attack>();
	attack->Init();
	attack->SetPos(m_pos + Math::Vector3{ 0.1f * m_sizeX,FLOAT_ZERO,FLOAT_ZERO });
	attack->SetOwner(m_owner);
	SceneManager::Instance().AddObject(attack);
}

void Player::UnableAct()
{
	m_apraxiaWait = 120;
	m_statysType = StatusType::Apraxia;
	m_color = kBlueColor;
}

void Player::SetHPDec(int _decNum)
{
	if (m_invinWait == INT_ZERO)
	{
		m_hurtFlg = true;
		m_HP -= _decNum;
		m_color = kRedColor;
		m_invinWait = 20;
	}
}

void Player::SetWild_Zombie(std::weak_ptr<Wild_Zombie> _wpw_Zombie)
{
	if (_wpw_Zombie.expired() == false)
	{
		m_wpw_Zombie = _wpw_Zombie;
	}
}

void Player::SetZombie_Woman(std::weak_ptr<Zombie_Woman> _wpZombie_w)
{
	if (_wpZombie_w.expired() == false)
	{
		m_wpZombie_w = _wpZombie_w;
	}
}

void Player::Death()
{
	m_pType = PlayerType::Dead;
}

void Player::Release()
{
	m_spPoly = nullptr;
}
