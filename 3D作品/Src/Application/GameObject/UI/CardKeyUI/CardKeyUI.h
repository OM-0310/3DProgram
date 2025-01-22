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

	void Use		() { m_useFlg = true; }

	void SetCardKey(const std::shared_ptr<CardKey>& _spCard)
	{
		m_wpCard = _spCard;
	}

private:

	std::weak_ptr<CardKey>	m_wpCard;

	int						m_lifeSpan		= 0;	// 生存期間
	const int				m_lifeSpanMin	= 0;	// 生存期間最小値

	bool					m_useFlg	= false;	// 使用フラグ
};
