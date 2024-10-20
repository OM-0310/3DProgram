#include "SecretFile.h"

#include "../../Scene/SceneManager.h"

void SecretFile::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelData>();
		m_spModel->Load("Asset/Models/SecretFile/SecretFile.gltf");
	}

	m_pos = {10.3f,0.24f,19.4f};

	m_color = kWhiteColor;
	m_alpha = 1.0f;
	m_alphaFlg = false;

	m_collectArea = 1.f;
	m_collectFlg = false;

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_debugColor = kRedColor;
	m_pDebugWire = std::make_unique<KdDebugWireFrame>();
}

void SecretFile::Update()
{
	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Radius = m_collectArea;
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
				m_collectFlg = true;
				m_debugColor = kGreenColor;
				break;
			}
			else
			{
				m_collectFlg = false;
				m_debugColor = kRedColor;

				break;
			}
		}
	}


	if (m_alphaFlg)
	{
		m_alpha += 0.025f;
		if (m_alpha >= 1.0f)
		{
			m_alphaFlg = false;
		}
	}
	else
	{
		m_alpha -= 0.025f;
		if (m_alpha <= 0.0f)
		{
			m_alphaFlg = true;
		}
	}
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void SecretFile::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void SecretFile::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void SecretFile::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
