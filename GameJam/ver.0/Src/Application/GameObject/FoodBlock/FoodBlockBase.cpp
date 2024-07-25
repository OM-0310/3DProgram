#include "FoodBlockBase.h"

void FoodBlockBase::Init()
{
}

void FoodBlockBase::Update()
{
}

void FoodBlockBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}
