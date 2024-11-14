#pragma once

class UIBase : public KdGameObject
{
public:

	UIBase					()				{}
	virtual ~UIBase			()	override	{ Release(); }

	virtual void Init		()	override	{}
	virtual void Update		()	override	{}
	virtual void DrawSprite	()	override	{}

	void SetAsset(std::string _name);

protected:

	enum class AlphaValue
	{
		Inc = 1 << 0,	// 加算
		Dec = 1 << 1	// 減算
	};

	void Release			()				{ m_spTex = nullptr; }

protected:

	std::shared_ptr<KdTexture>		m_spTex;		// 画像情報

	Math::Matrix					m_mScale;		// 拡縮行列
	
	Math::Rectangle					m_rect;			// 描画範囲

	Math::Vector2					m_pos;			// 座標

	Math::Color						m_color;		// 色情報

	float							m_alpha;		// アルファ値
	float							m_alphaSpeed;	// アルファスピード
	AlphaValue						m_alphaValue;	// アルファ値加減
};
