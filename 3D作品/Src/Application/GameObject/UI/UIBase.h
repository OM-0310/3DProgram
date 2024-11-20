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

	std::shared_ptr<KdTexture>		m_spTex		= nullptr;					// 画像情報

	Math::Matrix					m_mScale	= Math::Matrix::Identity;	// 拡縮行列
	
	Math::Rectangle					m_rect		= { 0,0,0,0 };				// 描画範囲

	Math::Vector2					m_pos		= Math::Vector2::Zero;		// 座標

	Math::Color						m_color		= kWhiteColor;				// 色情報

	float							m_alpha		= 1.0f;						// アルファ値
	float							m_alphaSpeed= 0.0f;						// アルファスピード
	AlphaValue						m_alphaValue= AlphaValue::Inc;			// アルファ値加減
};
