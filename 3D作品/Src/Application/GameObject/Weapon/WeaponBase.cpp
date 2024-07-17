#include "WeaponBase.h"
#include "../Characters/CharaBase.h"

void WeaponBase::Update()
{
	const std::shared_ptr<CharaBase> _spChara = m_wpChara.lock();
	Math::Matrix _charaMat;
	if (_spChara)
	{
		// 親の行列を取得
		_charaMat = _spChara->GetMatrix();

		// 親の行列を反映したワールド行列を作成
		m_mWorld = m_mScale * m_mRot * m_mLocal * _charaMat;
	}
}

void WeaponBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void WeaponBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
