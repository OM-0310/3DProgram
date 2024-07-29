#include "Player.h"

#include "../Food/Meet/Meet.h"
#include "../Food/Vegetable/Vegetable.h"
#include "../FoodBlock/MeetBlock/MeetBlock.h"
#include "../FoodBlock/VegetableBlock/VegetableBlock.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/GameScene/GameScene.h"

void Player::Init()
{
	m_spPoly	= std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Player.png");

	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	m_spPoly->SetSplit(4, 8);

	m_pos		= {};
	m_moveDir	= {};

	m_moveSpeed = 0.1f;
	m_getArea	= 1.f;

	m_keyFlg	= false;
	m_getFlg	= false;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Player::Update()
{ 
	m_spPoly->SetUVRect(24);

	// 移動処理
	MoveProcess();


	//==食材================================================================

	// 食材取得処理
	MeetGetProcess();	// 肉
	VegetaGetProcess();	// 野菜

	//======================================================================


	//==================================================================
	//球判定
	//==================================================================
	// 球判定用の変数
	KdCollider::SphereInfo sphere;
	// 球の中心位置を設定
	sphere.m_sphere.Center = m_pos + Math::Vector3(0, 0.5f, 0);
	// 球の半径を設定
	sphere.m_sphere.Radius = 0.4f;
	// 当たり判定をしたいタイプを設定
	sphere.m_type = KdCollider::TypeGround;

	// デバッグ用(後で消す)
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius);

	// 弾が当たったオブジェクトの情報を格納するリスト
	std::list<KdCollider::CollisionResult> retSphereList;

	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->Intersects(sphere, &retSphereList))
		{
		}
	}

	// 球リストから一番近いオブジェクトを検出
	float maxOverLap = 0;	// はみ出た球の長さ
	bool ishit = false;	// 当たっていたらtrue;
	Math::Vector3 hitDir;
	for (auto& ret : retSphereList)
	{
		if (maxOverLap < ret.m_overlapDistance)
		{
			maxOverLap	= ret.m_overlapDistance;
			hitDir		= ret.m_hitDir;
			ishit		= true;
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


	m_mWorld = Math::Matrix::CreateTranslation(m_pos);
}

void Player::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}

//==食材================================================================

void Player::MoveProcess()
{
	m_moveDir = Math::Vector3::Zero;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_moveDir += Math::Vector3::Backward;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_moveDir += Math::Vector3::Forward;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_moveDir += Math::Vector3::Left;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_moveDir += Math::Vector3::Right;
	}

	m_moveDir.Normalize();

	m_pos += m_moveDir * m_moveSpeed;
}

//======================================================================

//==食材================================================================

void Player::MeetGetProcess()
{
	std::shared_ptr<MeetBlock> spMeet_B	= m_wpMeetBlock.lock();

	Math::Vector3 meetVec;

	if (spMeet_B)
	{
		meetVec = spMeet_B->GetPos() - m_pos;
	}

	std::shared_ptr<Meet> meet;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_keyFlg)
		{
			// 食材を取得できる範囲内にいるかどうか
			if (meetVec.Length() < m_getArea)
			{
				// 食材を所持しているかどうか
				if (!m_getFlg)
				{
					meet = std::make_shared<Meet>();
					meet->Init();
					meet->SetOwner(m_owner);
					SceneManager::Instance().AddObject(meet);

					m_getFlg = true;
					m_keyFlg = true;
				}
			}
		}
	}
	else
	{
		m_keyFlg = false;
	}

	// デバッグ用(後で消す)
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = spMeet_B->GetPos();
	sphereInfo.m_sphere.Radius = m_getArea;
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kGreenColor);
}

//======================================================================

//==食材================================================================

void Player::VegetaGetProcess()
{
	std::shared_ptr<VegetableBlock>	spVegeta_B = m_wpVagetaBlock.lock();

	Math::Vector3 vegetaVec;

	if (spVegeta_B)
	{
		vegetaVec = spVegeta_B->GetPos() - m_pos;
	}

	std::shared_ptr<Vegetable> vegeta;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_keyFlg)
		{
			// 食材を取得できる範囲内にいるかどうか
			if (vegetaVec.Length() < m_getArea)
			{
				// 食材を所持しているかどうか
				if (!m_getFlg)
				{
					vegeta = std::make_shared<Vegetable>();
					vegeta->Init();
					vegeta->SetOwner(m_owner);
					SceneManager::Instance().AddObject(vegeta);

					m_getFlg = true;
					m_keyFlg = true;
				}
			}
		}
	}
	else
	{
		m_keyFlg = false;
	}

	// デバッグ用(後で消す)
	KdCollider::SphereInfo sphereInfo;
	sphereInfo.m_sphere.Center = spVegeta_B->GetPos();
	sphereInfo.m_sphere.Radius = m_getArea;
	m_pDebugWire->AddDebugSphere(sphereInfo.m_sphere.Center, sphereInfo.m_sphere.Radius, kGreenColor);
}
//======================================================================
