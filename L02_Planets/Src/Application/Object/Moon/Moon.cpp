#include "Moon.h"
#include "../Earth/Earth.h"
#include "../../main.h"

void Moon::Init()
{
	m_model.Load("Asset/Data/LessonData/Planets/moon.gltf");
	m_pos = { 0,0,1.5f };
}

void Moon::Update()
{
	static float deg = 0.0f;
	deg += 1.5f;
	if (deg >= 360.0f)
	{
		deg -= 360.0f;
	}

	m_rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(deg));
	m_transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_rotMat * m_transMat * m_earthMat;
}

void Moon::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(m_model, m_mWorld);
}
