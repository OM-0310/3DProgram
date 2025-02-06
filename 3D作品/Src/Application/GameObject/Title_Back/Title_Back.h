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

	enum
	{
		FeedOutFlg,
		KeyFlg
	};

private:

	std::shared_ptr<KdTexture>			m_spTex;

	Math::Rectangle						m_rect;

	Math::Vector2						m_pos		= Math::Vector2::Zero;

	Math::Color							m_color		= kWhiteColor;

	static constexpr float				m_alphaMax	= 1.0f;
	static constexpr float				m_alphaMin	= 0.0f;
	static constexpr float				m_alphaSpd	= 0.05f;
	float								m_alpha		= 0.0f;

	static constexpr short				m_totalEachFlg = 2;
	std::bitset<m_totalEachFlg>			m_bitsEachFlg;

	static constexpr float				m_volume	= 0.2f;
	std::shared_ptr<KdSoundInstance>	m_spSound;
};
