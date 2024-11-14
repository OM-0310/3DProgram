#include "Door_3.h"

#include "../../Characters/Player/Player.h"

#include "../../../Scene/SceneManager.h"

void Door_3::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Gimmicks/Door/Door.gltf");
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Gimmicks/Door/Door.gltf"));
	}

	m_debugColor = kRedColor;

	m_pos = { -3.8f,-0.9f,10.f };
	m_eventPos = { -3.9f,0.f,10.f };

	m_openAbleFlg = false;
	m_openFlg = false;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("DoorCollsion", m_spModel, KdCollider::TypeGround);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Door_3::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_eventPos;
	sphere.m_sphere.Radius = m_openArea;
	sphere.m_type = KdCollider::TypeEvent;

	// デバッグ用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, m_debugColor);

	bool isHit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypePlayer)
		{
			isHit = obj->Intersects(sphere, nullptr);

			if (isHit)
			{
				m_openAbleFlg = true;
				m_debugColor = kGreenColor;
			}
			else
			{
				m_openAbleFlg = false;
				m_debugColor = kRedColor;
			}
		}
	}

	if (m_openAbleFlg)
	{
		m_openFlg = true;
	}
	if (m_openFlg)
	{
		m_pos.y += m_moveSpeed;
		if (m_pos.y >= m_moveMax)
		{
			m_pos.y = m_moveMax;
			if (!m_openAbleFlg)
			{
				m_openFlg = false;
			}
		}
	}
	else
	{
		m_pos.y -= m_moveSpeed;
		if (m_pos.y <= m_moveMin)
		{
			m_pos.y = m_moveMin;
		}
	}

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;
}

void Door_3::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Door_3::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
