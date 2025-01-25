#pragma once

#include "../UIBase.h"

class MapUpdateUI : public UIBase
{
public:

	MapUpdateUI		()				{}
	~MapUpdateUI	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

private:

	short		m_actionCnt		= 0;		// アクションカウント(0以上の数字のみ使用するのでunsigned型で使用)
	const short	m_actionCntMax	= 120;		// アクションカウント最大値

	const float m_middlePosX	= 452.5f;	// X座標中間地点
	const float	m_arrivalPosX	= 827.5f;	// X座標到達地点
	const float m_moveSpeed		= 10.0f;	// 移動速度
};
