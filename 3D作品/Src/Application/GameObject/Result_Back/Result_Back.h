#pragma once

class Result_Back : public KdGameObject
{
public:

	Result_Back		()				{}
	~Result_Back	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetFeedOut(const bool& _feedOut) { m_feedOut = _feedOut; }

	const float& GetAlpha	() const { return m_alpha; }
	const float& GetAlphaMax() const { return m_alphaMax; }

private:

	std::shared_ptr<KdTexture>	m_spTex;

	Math::Rectangle				m_rect;

	Math::Vector2				m_pos = Math::Vector2::Zero;

	Math::Color					m_color = kWhiteColor;

	const float					m_alphaMax	= 1.0f;
	const float					m_alphaMin	= 0.0f;
	const float					m_alphaSpd	= 0.05f;
	float						m_alpha		= 0.0f;

	bool						m_feedOut	= false;
	bool						m_keyFlg	= false;
};
