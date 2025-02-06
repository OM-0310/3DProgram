#pragma once

class Player;

class Game_Back : public KdGameObject
{
public:

	Game_Back		()				{}
	~Game_Back		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetFeedOut(const bool& _feedOut) { m_bitsEachFlg[FeedOutFlg] = _feedOut; }

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const float& GetAlpha	() const { return m_alpha; }
	const float& GetAlphaMax() const { return m_alphaMax; }

private:

	enum
	{
		DisplayFlg,
		FeedOutFlg
	};

private:

	std::shared_ptr<KdTexture>	m_spTex;
	std::weak_ptr<Player>		m_wpPlayer;
	
	Math::Rectangle				m_rect;

	Math::Vector2				m_pos		= Math::Vector2::Zero;

	Math::Color					m_color		= kWhiteColor;

	static constexpr float		m_alphaMax	= 1.0f;
	static constexpr float		m_alphaMin	= 0.0f;
	static constexpr float		m_alphaSpd	= 0.05f;
	float						m_alpha		= 0.0f;

	static constexpr short		m_totalEachFlg = 2;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;
};
