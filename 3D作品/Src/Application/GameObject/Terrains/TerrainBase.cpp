#include "TerrainBase.h"

void TerrainBase::Init()
{
}

void TerrainBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void TerrainBase::SetAsset(std::string _name)
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		KdAssets::Instance().m_modeldatas.GetData(_name);
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData(_name));
	}
}
