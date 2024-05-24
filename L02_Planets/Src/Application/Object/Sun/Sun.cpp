#include "Sun.h"

void Sun::Init()
{
	m_model.Load("Asset/Data/LessonData/Planets/sun.gltf");
	m_pos = { 0,0,10 };
}

void Sun::Update()
{
	static float deg = 0.0f;
	deg += 0.5f;
	if (deg > 360)
	{
		deg -= 360;
	}
	m_rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(deg));
	m_transMat = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_rotMat * m_transMat;
}

void Sun::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(m_model, m_mWorld);
}
