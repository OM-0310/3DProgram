#include "ExclamationMarkUI.h"

#include "../../Characters/Player/Player.h"

void ExclamationMarkUI::Init()
{
	KdAssets::Instance().m_textures.GetData("Asset/Textures/UI/ExclamationMarkUI/ExclamationMarkUI.png");
	m_spPoly	= std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/UI/ExclamationMarkUI/ExclamationMarkUI.png"));

	m_mTrans	= Math::Matrix::Identity;
	m_mRot		= Math::Matrix::Identity;

	m_pos		= {};
	m_dir		= {};

	m_lifeSpan	= m_lifeSpanMax;
}

void ExclamationMarkUI::Update()
{
	const std::shared_ptr<Player>		spPlayer	= m_wpPlayer.lock();
	Math::Matrix playerMat;

	if (spPlayer)
	{
		playerMat = spPlayer->GetMatrix();
	}

	m_lifeSpan--;
	if (m_lifeSpan <= m_lifeSpanMin)
	{
		m_isExpired = true;
	}

	// プレイヤー(カメラ)と同じ軸を代入して方向を合わせる
	m_mWorld.Backward(playerMat.Backward());
	m_mWorld.Up(playerMat.Up());
	m_mWorld.Right(playerMat.Right());

	m_mWorld.Translation(m_pos + m_adjustPos);
}

void ExclamationMarkUI::DrawUnLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
