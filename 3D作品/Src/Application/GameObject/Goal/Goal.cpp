#include "Goal.h"

#include "../../Scene/SceneManager.h"

void Goal::Init()
{
	m_pos = { 0.f,0.f,-50.f };

	m_debugColor = kRedColor;

	m_goalAbleFlg = false;
	m_clearFlg = false;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Goal::Update()
{
	if (m_goalAbleFlg)
	{
		KdCollider::SphereInfo sphere;
		sphere.m_sphere.Center = m_pos;
		sphere.m_sphere.Radius = 0.5f;
		sphere.m_type = KdCollider::TypeBump;

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
					m_clearFlg = true;
					m_debugColor = kGreenColor;
				}
			}
		}
	}
}
