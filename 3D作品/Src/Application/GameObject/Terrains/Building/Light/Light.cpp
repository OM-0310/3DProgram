#include "Light.h"

void Light::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Building/Light/Light.gltf");

	m_pos = { 0.f,3.45f,0.f };

	m_objectType = KdGameObject::ObjectType::TypeObstacles;
}

void Light::Update()
{
	KdShaderManager::Instance().WorkAmbientController().AddPointLight({ 3.0f,3.0f,3.0f }, 8, m_pos + Math::Vector3{ 0.0f,-3.0f,0.0f }, true);

	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;
}

void Light::DrawLit()
{
	TerrainBase::DrawLit();
}

void Light::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, {1.0f,1.0f,1.0f,0.5f});
}
