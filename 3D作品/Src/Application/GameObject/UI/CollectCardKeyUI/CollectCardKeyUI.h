#pragma once

#include "../UIBase.h"

class CollectCardKeyUI : public UIBase
{
public:

	CollectCardKeyUI	()				{}
	~CollectCardKeyUI	()	override	{}

	void Init			()	override;
	void Update			()	override;
	void DrawSprite		()	override;

private:

	short						m_actionCnt		= 0;		// アクションカウント(0以上の数字のみ使用するのでunsigned型で使用)
	static constexpr short		m_actionCntMax	= 120;		// アクションカウント最大値

	static constexpr float		m_middlePosX	= 496.0f;	// X座標中間地点
	static constexpr float		m_arrivalPosX	= 784.0f;	// X座標到達地点
	static constexpr float		m_moveSpeed		= 10.0f;	// 移動速度
};
