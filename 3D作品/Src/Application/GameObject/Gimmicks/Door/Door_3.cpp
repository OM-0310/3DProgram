﻿#include "Door_3.h"

#include "../../../Scene/SceneManager.h"

void Door_3::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Gimmicks/Door/Door.gltf");
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Gimmicks/Door/Door.gltf"));
	}

	m_spOpenSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/OpenDoor.wav");
	m_spOpenSound->SetVolume(m_openVol);

	m_debugColor = kRedColor;

	m_pos = { -3.8f,-0.9f,10.f };
	m_eventPos = { -3.9f,0.f,10.f };

	for (uint16_t i = 0; i < m_bitsEachFlg.size(); ++i)
	{
		m_bitsEachFlg.reset(i);
	}

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("DoorCollsion", m_spModel, KdCollider::TypeGround | KdCollider::TypeBump);

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Door_3::Update()
{
	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_eventPos;
	sphere.m_sphere.Radius = m_openArea;
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
				m_bitsEachFlg.set(OpenAbleFlg, true);
				m_debugColor = kGreenColor;
			}
			else
			{
				m_bitsEachFlg.set(OpenAbleFlg, false);
				m_debugColor = kRedColor;
			}
		}
	}

	if (m_bitsEachFlg.test(OpenAbleFlg))
	{
		m_bitsEachFlg.set(OpenFlg, true);

		if (m_spOpenSound->IsStopped())
		{
			m_spOpenSound->Play();
		}
	}
	if (m_bitsEachFlg.test(OpenFlg))
	{
		m_pos.y += m_moveSpeed;
		if (m_pos.y >= m_moveMax)
		{
			m_pos.y = m_moveMax;
			if (!m_bitsEachFlg.test(OpenAbleFlg))
			{
				m_bitsEachFlg.set(OpenFlg, false);

				m_spOpenSound->Play();
				
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

	m_spOpenSound->SetPos(GetPos());
}

void Door_3::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void Door_3::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
