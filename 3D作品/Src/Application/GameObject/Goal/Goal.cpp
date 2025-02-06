#include "Goal.h"

#include "../Characters/Player/Player.h"

#include "../../Scene/SceneManager.h"

void Goal::Init()
{
	if (!m_spPoly)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/UI/GoalUI/GoalUI.png");
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/UI/GoalUI/GoalUI.png"));
		m_spPoly->SetScale(3.0f);
	}

	m_pos = { 40.0f,-0.9f,-60.f };

	m_debugColor = kRedColor;

	m_bitsEachFlg[GoalAbleFlg] = false;

	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void Goal::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();

	if (m_bitsEachFlg[GoalAbleFlg])
	{
		KdCollider::SphereInfo sphere;
		sphere.m_sphere.Center = m_pos;
		sphere.m_sphere.Radius = 0.8f;
		sphere.m_type = KdCollider::TypeBump;

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
						spPlayer->SetFeedOutFlg(true);
					}
				}
			}
		}
	}

	m_mRot		= Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld	= m_mRot * m_mTrans;
}

void Goal::DrawUnLit()
{
	if (!m_bitsEachFlg[GoalAbleFlg])return;
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
