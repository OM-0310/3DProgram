#pragma once

#include "../UIBase.h"

class MissionDisplayUI : public UIBase
{
public:

	MissionDisplayUI	()				{}
	~MissionDisplayUI	()	override	{}

	void Init			()	override;
	void Update			()	override;
	void DrawSprite		()	override;

private:
	enum
	{
		FeedOutFlg,
		lifeDecFlg
	};

private:

	static constexpr short		m_lifeSpanMax	= 60;
	static constexpr short		m_lifeSpanMin	= 0;
	short						m_lifeSpan		= 0;

	static constexpr short		m_totalEachFlg	= 2;
	std::bitset<m_totalEachFlg>	m_bitsEachFlg;
};
