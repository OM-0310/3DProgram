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

void WeaponBase::SetAsset(std::string _name)
{
	//// //// //// //// //// //// //// //// //// //// ////
	// 使用するアセットをセットする関数					//
	// (同じアセットを使用する可能性があるため、		//
	// 学校側が用意しているフライウェイトパターンを		//
	// 用いて実装されているKdAssetsクラスを使用)		//
	//// //// //// //// //// //// //// //// //// //// ////

	// モデル情報がなければ
	if (!m_spModel)
	{
		// モデル情報を実体化
		m_spModel = std::make_shared<KdModelWork>();
		// KdAssetsにアセットを格納
		KdAssets::Instance().m_modeldatas.GetData(_name);
		// KdAssets内にあるモデル情報を格納
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData(_name));
	}
}
