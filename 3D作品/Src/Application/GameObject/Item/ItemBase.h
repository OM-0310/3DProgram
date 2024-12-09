#pragma once

class Player;

class ItemBase : public KdGameObject
{
public:

	ItemBase								()				{}
	virtual ~ItemBase						()	override	{}

	virtual void Init						()	override;
	virtual void Update						()	override;
	virtual void DrawLit					()	override;
	virtual void DrawBright					()	override;
	virtual void GenerateDepthMapFromLight	()	override;

	void UpdateCollision					();						// プレイヤーとの当たり判定
	void Flickering							(float _alphaSpeed);	// モデルの点滅処理

	void Extinction							() { m_isExpired = true; }

	void SetAsset(std::string _name);

	const bool& GetCollectFlg				() { return m_collectFlg; }

protected:

	std::shared_ptr<KdModelData>	m_spModel;			// モデル情報

	Math::Matrix	m_mTrans = Math::Matrix::Identity;	// 座標行列

	Math::Color		m_color			= kWhiteColor;	// 色情報
	float			m_alpha			= 1.f;			// アルファ値
	const float		m_alphaMax		= 1.f;			// アルファ最大値 = 1.0f
	const float		m_alphaMin		= 0.f;			// アルファ最小値 = 0.0f
	float			m_alphaSpeed	= 0.f;			// アルファ値加減速度
	bool			m_alphaFlg		= false;		// アルファ値加減速フラグ

	Math::Vector3	m_pos = Math::Vector3::Zero;	// 座標

	float			m_collectArea	= 0.f;			// アイテム回収範囲
	bool			m_collectFlg	= false;		// アイテム回収フラグ

	// デバッグ用
	Math::Color		m_debugColor	= kWhiteColor;	// デバッグワイヤー用カラー
};
