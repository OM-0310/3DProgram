#include "Pistol_Ready.h"

#include "../../../Bullet/Bullet.h"
#include "../../../Characters/Player/Player.h"

#include "../../../../Scene/SceneManager.h"

void Pistol_Ready::Init()
{
	WeaponBase::SetAsset("Asset/Models/Weapon/Pistol/Pistol_Ready.gltf");

	m_alpha		= 0.0f;
	m_color		= { 1.f,1.f,1.f,m_alpha };
	m_shotFlg	= false;
	m_mLocal	= Math::Matrix::CreateTranslation({ 0.f,1.48f,0.4f });

	if (m_spModel)
	{
		const KdModelWork::Node* pNode = m_spModel->FindNode("MuzzlePoint");

		if (pNode)
		{
			m_mLocalMuzzle = pNode->m_worldTransform * m_mLocal;
		}
		else
		{
			assert(pNode && "MuzzlePoint 指定したノードが見つかりません");
		}

		m_pDebugWire = std::make_unique<KdDebugWireFrame>();
	}
}

void Pistol_Ready::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	if (spPlayer)
	{
		//// blender側で作成したノードを取得
		//KdModelData::Node* pNode = spPlayer->GetModel()->GetData()->FindNode("ReadyPoint");

		//// ノードのワールド行列を格納
		//Math::Matrix worldPos = pNode->m_worldTransform;

		//// 親行列の取得
		//Math::Matrix parentMat = spPlayer->GetMatrix();

		//// blender側のワールド行列と親行列を合成してゲーム内のワールド座標を取得
		//Math::Matrix gameWorldPos = worldPos * parentMat;

		//KdCollider::SphereInfo sphere;
		//sphere.m_sphere.Center = gameWorldPos.Translation();
		//sphere.m_sphere.Radius = 0.05f;
		//sphere.m_type = KdCollider::TypeDamage;

		//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kBlueColor);

		//// 取得したワールド座標を行列に格納
		//m_mWorld = gameWorldPos;

		/*Math::Matrix rotMat;
		rotMat = spPlayer->GetRotateMat();

		Math::Matrix transMat;
		m_disarmPos = spPlayer->GetDisarmPos();
		transMat = Math::Matrix::CreateTranslation(m_disarmPos);

		m_mWorld = rotMat * transMat;*/

		if (spPlayer->GetHoldFlg())
		{
			m_alpha = 0.f;
		}
		else
		{
			m_alpha = 1.f;
		}
		m_color = { 1.f,1.f,1.f,m_alpha };
	}
	else
	{
		assert("spPlayer 情報がありません");
	}

	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, kBlueColor);	

	//if (m_shotFlg)
	//{
	//	KdCollider::RayInfo ray;
	//	//ray.m_pos = muzzlePos;
	//	//ray.m_dir = parentMat.Backward();
	//	ray.m_range = 1000.0f;
	//	ray.m_type = KdCollider::TypeDamage | KdCollider::TypeGround;

	//	//m_pDebugWire->AddDebugLine(ray.m_pos, ray.m_dir, ray.m_range);

	//	std::list<KdCollider::CollisionResult> retRayList;

	//	for (auto& obj : SceneManager::Instance().GetObjList())
	//	{
	//		obj->Intersects(ray, &retRayList);
	//	}

	//	float maxOverLap = 0.0f;
	//	Math::Vector3 hitPos = {};

	//	for (auto& ret : retRayList)
	//	{
	//		if (maxOverLap < ret.m_overlapDistance)
	//		{
	//			maxOverLap = ret.m_overlapDistance;
	//			hitPos = ret.m_hitPos;
	//		}
	//	}

	//	Math::Vector3 bulletDir = hitPos - muzzlePos;

	//	std::shared_ptr<Bullet> bullet;
	//	bullet = std::make_shared<Bullet>();
	//	bullet->Init();
	//	bullet->Shot(muzzlePos, bulletDir);
	//	SceneManager::Instance().AddObject(bullet);

	//	m_shotFlg = false;
	//}
}

void Pistol_Ready::DrawLit()
{
	WeaponBase::DrawLit();
}

void Pistol_Ready::GenerateDepthMapFromLight()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		if (spPlayer->GetHoldFlg())
		{
			WeaponBase::GenerateDepthMapFromLight();
		}
	}
}

void Pistol_Ready::ShotBullet()
{
	m_shotFlg = true;
}
