#pragma once

#include "../UIBase.h"

class AnswerUIBack : public UIBase
{
public:

	AnswerUIBack	()				{}
	~AnswerUIBack	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

private:

	static constexpr short			m_lifeSpanMax	= 60;
	static constexpr short			m_lifeSpanMin	= 0;
	short							m_lifeSpan		= 0;
};
