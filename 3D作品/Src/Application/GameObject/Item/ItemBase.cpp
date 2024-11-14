#include "ItemBase.h"

#include "../../Scene/SceneManager.h"

void ItemBase::Init()
{
}

void ItemBase::Update()
{
}

void ItemBase::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void ItemBase::DrawBright()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void ItemBase::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void ItemBase::UpdateCollision()
{
	//// //// //// //// //// //// //// //// //// //// ////
	// プレイヤーとの当たり判定(円判定を用いて実装)		//
	//// //// //// //// //// //// //// //// //// //// ////

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_pos;
	sphere.m_sphere.Radius = m_collectArea;
	sphere.m_type = KdCollider::TypeEvent;

	// デバッグ用
	m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, m_debugColor);

	bool isHit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// オブジェクトタイプがプレイヤーだったら
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypePlayer)
		{
			// 当たり判定を行う
			isHit = obj->Intersects(sphere, nullptr);

			// ヒットフラグがtrueであれば
			if (isHit)
			{
				// アイテム回収フラグをtrueにしループを抜ける
				m_collectFlg = true;
				m_debugColor = kGreenColor;
				break;
			}
			// falseであれば
			else
			{
				// アイテム回収フラグをfalseにしループを抜ける
				m_collectFlg = false;
				m_debugColor = kRedColor;

				break;
			}
		}
	}
}

void ItemBase::Flickering(float _alphaSpeed)
{
	//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ///
	// モデルの点滅処理(DrawBlightで描画したモデルを用いる。引数でアルファ値の加減速度を設定可能)	//
	//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ///

	// アルファ値加減速フラグがtrueであれば
	if (m_alphaFlg)
	{
		// アルファ値をプラスする
		m_alpha += _alphaSpeed;
		// アルファ値が最大値になると
		if (m_alpha >= m_alphaMax)
		{
			// アルファ値を最大値にする
			m_alpha		= m_alphaMax;
			// アルファ値加減速フラグをfalseにする
			m_alphaFlg	= false;
		}
	}
	// falseであれば
	else
	{
		// アルファ値をマイナスする
		m_alpha -= _alphaSpeed;
		// アルファ値が最小値になると
		if (m_alpha <= m_alphaMin)
		{
			// アルファ値を最小値にする
			m_alpha		= m_alphaMin;
			// アルファ値加減速フラグをtrueにする
			m_alphaFlg	= true;
		}
	}

	// アルファ値を確定
	m_color = { 1.f,1.f,1.f,m_alpha };
}

void ItemBase::SetAsset(std::string _name)
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
		m_spModel = std::make_shared<KdModelData>();
		// KdAssetsにアセットを格納
		KdAssets::Instance().m_modeldatas.GetData(_name);
		// KdAssets内にあるモデル情報を格納
		m_spModel = KdAssets::Instance().m_modeldatas.GetData(_name);
	}
}
