#pragma once

class Title_Back : public KdGameObject
{
public:

	Title_Back		()				{}
	~Title_Back		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	const float& GetAlpha	() const { return m_alpha; }
	const float& GetAlphaMax() const { return m_alphaMax; }

private:

	std::shared_ptr<KdTexture>			m_spTex;

	Math::Rectangle						m_rect;

	Math::Vector2						m_pos		= Math::Vector2::Zero;

	Math::Color							m_color		= kWhiteColor;

	const float							m_alphaMax	= 1.0f;
	const float							m_alphaMin	= 0.0f;
	const float							m_alphaSpd	= 0.05f;
	float								m_alpha		= 0.0f;

	bool								m_feedOut	= false;
	bool								m_keyFlg	= false;

	const float							m_volume	= 0.3f;
	std::shared_ptr<KdSoundInstance>	m_spSound;
};
