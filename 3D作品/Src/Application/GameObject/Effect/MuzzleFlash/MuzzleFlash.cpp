#include "MuzzleFlash.h"

#include "../../Characters/Player/Player.h"

void MuzzleFlash::Init()
{
	KdAssets::Instance().m_textures.GetData("Asset/Textures/Effect/MuzzleFlash/MuzzleFlash.png");
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/Effect/MuzzleFlash/MuzzleFlash.png"));

	m_mTrans = Math::Matrix::Identity;
	m_mRot = Math::Matrix::Identity;

	m_pos = {};
	m_dir = {};

	m_lifeSpan = m_lifeSpanMax;
}

void MuzzleFlash::Update()
{
	const std::shared_ptr<Player>		spPlayer = m_wpPlayer.lock();
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

	m_mWorld.Translation(m_pos);
}

void MuzzleFlash::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
}
