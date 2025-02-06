#pragma once

class SelectTitle;
class SelectRetry;

class GameOver_Back : public KdGameObject
{
public:

	GameOver_Back	()				{}
	~GameOver_Back	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetFeedOut(const bool& _feedOut) { m_bitsEachFlg.set(FeedOutFlg, _feedOut); }

	void SetSelectTitle(const std::shared_ptr<SelectTitle>& _spTitle)
	{
		m_wpTitle = _spTitle;
	}

	void SetSelectRetry(const std::shared_ptr<SelectRetry>& _spRetry)
	{
		m_wpRetry = _spRetry;
	}

	const float& GetAlpha	() const { return m_alpha;		}
	const float& GetAlphaMax() const { return m_alphaMax;	}

private:

	enum
	{
		AKeyFlg,	// Aキーフラグ
		DKeyFlg,	// Dキーフラグ
		EnterKeyFlg,// エンターキーフラグ
		FeedOutFlg	// フェードアウトフラグ
	};

private:

	std::shared_ptr<KdTexture>	m_spTex;
	std::weak_ptr<SelectTitle>	m_wpTitle;
	std::weak_ptr<SelectRetry>	m_wpRetry;

	Math::Rectangle				m_rect;

	Math::Vector2				m_pos			= Math::Vector2::Zero;

	Math::Color					m_color			= kWhiteColor;

	static constexpr float		m_alphaMax		= 1.0f;
	static constexpr float		m_alphaMin		= 0.0f;
	static constexpr float		m_alphaSpd		= 0.05f;
	float						m_alpha			= 0.0f;

	static constexpr short		m_totalEachFlg	= 4;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;

	static constexpr float				m_selectVol = 0.2f;
	std::shared_ptr<KdSoundInstance>	m_spSelectSound;

	static constexpr float				m_pressVol	= 0.2f;
	std::shared_ptr<KdSoundInstance>	m_spPressSound;
};
