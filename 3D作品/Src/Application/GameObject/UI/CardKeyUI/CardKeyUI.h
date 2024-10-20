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

	void Extinction() { m_isExpired = true; }

	void SetCardKey(const std::shared_ptr<CardKey>& _spCard)
	{
		m_wpCard = _spCard;
	}

private:

	std::weak_ptr<CardKey> m_wpCard;
};
