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

	enum ProgressStateType
	{
		Stop		= 1 << 0,	// 停止
		Progress	= 1 << 1,	// 前進
		Retreat		= 1 << 2	// 後退
	};

	enum AlphaStateType
	{
		Inc = 1 << 0,	// 加算
		Dec = 1 << 1	// 減算
	};

	void Release			()				{ m_spTex = nullptr; }

protected:

	std::shared_ptr<KdTexture>		m_spTex			= nullptr;						// 画像情報
	
	Math::Rectangle					m_rect			= { 0,0,0,0 };					// 描画範囲

	Math::Color						m_color			= kWhiteColor;					// 色情報

	Math::Vector2					m_pivot			= Math::Vector2::Zero;			// 枢軸

	Math::Vector2					m_pos			= Math::Vector2::Zero;			// 座標

	float							m_alpha			= 1.0f;							// アルファ値
	float							m_alphaSpeed	= 0.0f;							// アルファスピード
	static constexpr float			m_alphaMax		= 1.0f;							// アルファ値最大値
	static constexpr float			m_alphaMin		= 0.0f;							// アルファ値最小値
	UINT							m_alphaState	= AlphaStateType::Inc;			// アルファ値加減

	UINT							m_progressState = ProgressStateType::Stop;		// 進行状態
};
