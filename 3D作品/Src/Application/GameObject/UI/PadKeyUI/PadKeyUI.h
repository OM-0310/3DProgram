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

	std::weak_ptr<LockedDoor>	m_wpLockedDoor;		// 鍵のかかったドア情報

	bool						m_useFlg;			// フラグ

	int							m_lifeSpan;			// 生存期間
	int							m_singleX;			// アニメーション時の1つの画像サイズ

	const int					m_animMax = 12;		// アニメーションする画像の総数
	float						m_animCnt;			// アニメーションカウント
};
