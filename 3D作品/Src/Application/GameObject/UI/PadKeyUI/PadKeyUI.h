#pragma once

#include "../UIBase.h"

class LockedDoor;

class PadKeyUI : public UIBase
{
public:

	PadKeyUI		()				{}
	~PadKeyUI		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetLockedDoor(const std::shared_ptr<LockedDoor>& _spLockedDoor)
	{
		m_wpLockedDoor = _spLockedDoor;
	}

private:

	enum
	{
		UseFlg
	};

private:

	std::weak_ptr<LockedDoor>	m_wpLockedDoor;				// 鍵のかかったドア情報

	static constexpr short		m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg>	m_bitsEachFlg;

	static constexpr short		m_lifeSpanMax	= 35;		// 生存期間最大値 35
	static constexpr short		m_lifeSpanMin	= 0;		// 生存期間最小値 0
	short						m_lifeSpan		= 0;		// 生存期間

	short						m_singleX		= 0;		// アニメーション時の1つの画像サイズ

	static constexpr float		m_animMax		= 12.0f;	// アニメーションする画像の総数
	static constexpr float		m_animSpeed		= 0.5f;		// アニメーション速度
	float						m_animCnt		= 0.0f;		// アニメーションカウント
};
