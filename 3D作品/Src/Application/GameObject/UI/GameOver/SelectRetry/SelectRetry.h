#pragma once

class SelectRetry : public KdGameObject
{
public:

	SelectRetry		()				{}
	~SelectRetry	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetChoiseFlg(const bool& _choiceFlg) { m_bitsEachFlg[ChoiceFlg] = _choiceFlg; }

	const bool GetChoiceFlg() const { return m_bitsEachFlg.test(ChoiceFlg); }

private:

	enum
	{
		ChoiceFlg
	};

private:

	std::shared_ptr<KdTexture>	m_spTex;

	Math::Rectangle				m_rect;

	Math::Vector2				m_pos		= Math::Vector2::Zero;

	Math::Color					m_color		= kWhiteColor;

	const float					m_alphaMax	= 1.0f;
	const float					m_alphaHalf	= 0.5f;
	const float					m_alphaSpd	= 0.05f;
	float						m_alpha		= 0.0f;

	static constexpr short		m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;
};
