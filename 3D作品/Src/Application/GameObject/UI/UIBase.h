#pragma once

class UIBase : public KdGameObject
{
public:

	UIBase					()				{}
	~UIBase					()	override	{ Release(); }

	virtual void Init		()	override	{}
	virtual void Update		()	override	{}
	virtual void DrawSprite	()	override	{}

protected:

	void Release			()				{ m_tex.Release(); }

protected:

	KdTexture		m_tex;		// 画像情報
	
	Math::Vector2	m_pos;		// 座標

	Math::Color		m_color;	// 色情報

	float			m_alpha;	// アルファ値
};
