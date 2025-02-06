#pragma once

#include "../UIBase.h"

class PressEnterUI : public UIBase
{
public:

	PressEnterUI	() {}
	~PressEnterUI	()	override {}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

private:

	enum
	{
		FeedOutFlg
	};

private:

	static constexpr short		m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;

	const float m_alphaSpd = 0.01f;
};
