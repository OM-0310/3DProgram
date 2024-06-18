#include "Stage.h"

void Stage::Init()
{
	// データ読み込み
	m_spModel = std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/Stage/Stage.gltf");

	Math::Matrix transMat = Math::Matrix::CreateTranslation(0, -1.5f , 1.5f);
	Math::Matrix scaleMat = Math::Matrix::CreateScale(1.0f);
	Math::Matrix rotMat = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(90));

	m_mWorld = scaleMat * rotMat * transMat;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("StageCollision", m_spModel, KdCollider::TypeGround);
}

void Stage::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
