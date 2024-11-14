#include "Water.h"

void Water::Init()
{
	m_spModel	= std::make_shared<KdModelWork>();
	m_spModel->SetModelData("Asset/LessonData/Stage/Water/Water.gltf");

	m_pos		= { 0.0f,0.5f,0.0f };

	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld	= m_mTrans;
}

void Water::Update()
{
	m_UVOffset.x += 0.0001f;
	m_UVOffset.y += 0.0001f;
	m_mR = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(90));
}

void Water::DrawLesson()
{
	KdShaderManager::Instance().m_LessonShader.SetUVOffset(m_UVOffset);
	KdShaderManager::Instance().m_LessonShader.SetmR(m_mR);
	KdShaderManager::Instance().m_LessonShader.DrawModel(*m_spModel, m_mWorld);
}
