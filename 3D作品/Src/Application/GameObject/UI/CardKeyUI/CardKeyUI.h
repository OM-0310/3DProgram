#pragma once

#include "../UIBase.h"

class CardKey;

class CardKeyUI : public UIBase
{
public:

	CardKeyUI		()				{}
	~CardKeyUI		()	override	{}
	
	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void Use		() { m_bitsEachFlg.set(UseFlg, true); }

	void SetCardKey(const std::shared_ptr<CardKey>& _spCard)
	{
		m_wpCard = _spCard;
	}

private:

	enum
	{
		UseFlg
	};

private:

	std::weak_ptr<CardKey>		m_wpCard;

	static constexpr short		m_lifeSpanMax	= 35;		// 生存期間最大値
	static constexpr short		m_lifeSpanMin	= 0;		// 生存期間最小値
	short						m_lifeSpan		= 0;		// 生存期間

	static constexpr short		m_totalEachFlg	= 1;
	std::bitset<m_totalEachFlg>	m_bitsEachFlg	= false;	// 使用フラグ
};
