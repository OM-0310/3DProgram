#include "Grass.h"

void Grass::Init()
{
	TerrainBase::SetAsset("Asset/Models/Terrains/Grass/Grass.gltf");

	m_pos = { 0.f,-1.f,0.f };

	m_objectType = KdGameObject::ObjectType::TypeObstacles;
}

void Grass::Update()
{
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;
}

void Grass::DrawLit()
{
	TerrainBase::DrawLit();
}

void Grass::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
