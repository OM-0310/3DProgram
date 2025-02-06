#pragma once

#include "../UIBase.h"

class MiniMapUIBack : public UIBase
{
public:

	MiniMapUIBack	()				{}
	~MiniMapUIBack	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void Open		(const bool _active) { m_bitsEachFlg.set(ActiveFlg, _active); }

	const bool GetActive()const { return m_bitsEachFlg.test(ActiveFlg); }

private:

	enum
	{
		ActiveFlg
	};

private:

	static constexpr short		m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg>	m_bitsEachFlg;
};
