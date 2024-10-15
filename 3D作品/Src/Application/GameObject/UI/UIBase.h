#pragma once

class UIBase : public KdGameObject
{
public:

	UIBase					()				{}
	virtual ~UIBase			()	override	{ Release(); }

	virtual void Init		()	override	{}
	virtual void Update		()	override	{}
	virtual void DrawSprite	()	override	{}

protected:

	void Release			()				{ m_spTex = nullptr; }

protected:

	std::shared_ptr<KdTexture>		m_spTex;	// 画像情報
	
	Math::Vector2					m_pos;		// 座標

	Math::Color						m_color;	// 色情報

	float							m_alpha;	// アルファ値
};
