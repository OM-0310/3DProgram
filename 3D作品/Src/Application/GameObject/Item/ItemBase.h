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

	void SetPlayer(std::shared_ptr<Player> _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const bool& GetCollectFlg				() { return m_collectFlg; }

protected:

	std::shared_ptr<KdModelData>	m_spModel;			// モデル情報
	std::weak_ptr<Player>			m_wpPlayer;			// プレイヤー情報

	Math::Matrix					m_mTrans;			// 座標行列

	Math::Color						m_color;			// 色情報
	float							m_alpha;			// アルファ値
	const float						m_alphaMax = 1.0f;	// アルファ最大値 = 1.0f
	const float						m_alphaMin = 0.0f;	// アルファ最小値 = 0.0f
	float							m_alphaSpeed;		// アルファ値加減速度
	bool							m_alphaFlg;			// アルファ値加減速フラグ

	Math::Vector3					m_pos;				// 座標

	float							m_collectArea;		// アイテム回収範囲
	bool							m_collectFlg;		// アイテム回収フラグ

	// デバッグ用
	Math::Color						m_debugColor;		// デバッグワイヤー用カラー
};
