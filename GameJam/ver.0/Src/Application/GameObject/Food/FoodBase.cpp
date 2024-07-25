#include "FoodBase.h"

void FoodBase::Init()
{
}

void FoodBase::Update()
{
}

void FoodBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
