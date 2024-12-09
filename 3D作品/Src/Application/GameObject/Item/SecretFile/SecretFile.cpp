#include "SecretFile.h"

#include "../../../Scene/SceneManager.h"

void SecretFile::Init()
{
	ItemBase::SetAsset("Asset/Models/SecretFile/SecretFile.gltf");

	m_pos			= {10.3f,0.24f,19.4f};

	m_color			= kWhiteColor;
	m_alphaSpeed	= 0.025f;
	m_alpha			= 1.0f;
	m_alphaFlg		= false;

	m_collectArea	= 1.f;
	m_collectFlg	= false;

	m_mTrans		= Math::Matrix::CreateTranslation(m_pos);
	m_mWorld		= m_mTrans;

	m_debugColor	= kRedColor;
	m_pDebugWire	= std::make_unique<KdDebugWireFrame>();
}

void SecretFile::Update()
{
	ItemBase::UpdateCollision();

	Flickering(m_alphaSpeed);
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
