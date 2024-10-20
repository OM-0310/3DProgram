#include "TerrainBase.h"

void TerrainBase::Init()
{
}

void TerrainBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
