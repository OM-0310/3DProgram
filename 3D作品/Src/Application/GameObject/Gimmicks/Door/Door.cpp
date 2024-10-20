#include "Door.h"

#include "../../Characters/Player/Player.h"

#include "../../../Scene/SceneManager.h"

void Door::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/Gimmicks/Door/Door.gltf");
	}

	m_debugColor = kRedColor;

	m_pos		= { 0.f,-1.f,0.f };
	m_eventPos = { 3.f,0.f,-18.f };

	m_openAbleFlg	= false;
	m_openFlg		= false;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("DoorCollsion", m_spModel, KdCollider::TypeGround);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Door::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_eventPos;
	sphere.m_sphere.Radius = 0.8f;
	sphere.m_type = KdCollider::TypeEvent;

	// デバッグ用
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, m_debugColor);

	bool isHit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypePlayer)
		{
			isHit = obj->Intersects(sphere, nullptr);

			if (isHit)
			{
				if (spPlayer)
				{
					if (spPlayer->GetItemCollType() == Player::ItemCollectType::CardKeyCollect)
					{
						m_openAbleFlg = true;
						m_debugColor = kGreenColor;
					}
					else
					{
						m_openAbleFlg = false;
					}
				}
				break;
			}
			else
			{
				m_debugColor = kRedColor;

				break;
			}
		}
	}

	if (m_openFlg)
	{
		m_pos.y += 0.05f;
	}
	if (m_pos.y >= 2.5f)
	{
		m_isExpired = true;
	}


	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;
}

void Door::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Door::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Door::Open()
{
	m_openFlg = true;
}
