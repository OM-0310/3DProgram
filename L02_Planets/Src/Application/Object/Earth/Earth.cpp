#include "Earth.h"
#include "../Sun/Sun.h"

void Earth::Init()
{
	m_model.Load("Asset/Data/LessonData/Planets/earth.gltf");
	m_pos = { 0,0,4 };
}

void Earth::Update()
{
	static float deg = 0.0f;
	deg += 1.0f;
	if (deg >= 360.0f)
	{
		deg -= 360.0f;
	}

	m_rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(deg));
	m_transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_rotMat * m_transMat * m_sunMat;
}

void Earth::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(m_model, m_mWorld);
}
