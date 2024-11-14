#include "UIBase.h"

void UIBase::SetAsset(std::string _name)
{
	//// //// //// //// //// //// //// //// //// //// ////
	// 使用するアセットをセットする関数					//
	// (同じアセットを使用する可能性があるため、		//
	// 学校側が用意しているフライウェイトパターンを		//
	// 用いて実装されているKdAssetsクラスを使用)		//
	//// //// //// //// //// //// //// //// //// //// ////

	// 画像情報が無ければ
	if (!m_spTex)
	{
		// 画像情報を実体化
		m_spTex = std::make_shared<KdTexture>();
		// KdAssetsにアセットを格納
		KdAssets::Instance().m_textures.GetData(_name);
		// KdAssets内にある画像情報を格納
		m_spTex = KdAssets::Instance().m_textures.GetData(_name);
	}
}
