#include "WeaponBase.h"

#include "../../Characters/Player/Player.h"

#include "../../Characters/CharaBase.h"

void WeaponBase::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	Math::Matrix playerMat;
	if (spPlayer)
	{
		// 親の行列を取得
		playerMat = spPlayer->GetMatrix();

		// 親の行列を反映したワールド行列を作成
		m_mWorld = m_mScale * m_mRot * m_mLocal * playerMat;
	}
}

void WeaponBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}

void WeaponBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld, m_color);
}
