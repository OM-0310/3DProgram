#pragma once

#include "../UIBase.h"

class CollectUIBack : public UIBase
{
public:

	CollectUIBack	()				{}
	~CollectUIBack	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

private:

	UINT		m_actionCnt		= 0;		// アクションカウント(0以上の数字のみ使用するのでunsigned型で使用)
	const UINT	m_actionCntMax	= 120;		// アクションカウント最大値

	const float m_middlePosX	= 496.0f;	// X座標中間地点
	const float	m_arrivalPosX	= 784.0f;	// X座標到達地点
	const float m_moveSpeed		= 10.0f;	// 移動速度
};
