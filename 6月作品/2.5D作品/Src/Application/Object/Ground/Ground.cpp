#include "Ground.h"

void Ground::Init()
{
	// データ読み込み
	m_spModel = std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/Ground/Ground.gltf");

	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, 0, 1);

	m_mWorld = transMat;
}

void Ground::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
