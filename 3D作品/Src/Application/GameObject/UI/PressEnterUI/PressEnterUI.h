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

	bool m_feedOut = false;

	const float m_alphaSpd = 0.01f;
};
