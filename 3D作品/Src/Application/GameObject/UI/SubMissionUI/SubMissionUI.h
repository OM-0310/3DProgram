#pragma once

#include "../UIBase.h"

class SubMissionUI : public UIBase
{
public:

	SubMissionUI	()				{}
	~SubMissionUI	()	override	{}

	void Init		()	override;
	void DrawSprite	()	override;

	void Permission	() { m_bitsEachFlg[PermissionFlg] = true; }
	void Open(const bool _active) 
	{ 
		if (!m_bitsEachFlg[PermissionFlg]) { return; }
		m_bitsEachFlg[ActiveFlg] = _active;
	}

	const bool GetActive() const { return m_bitsEachFlg.test(ActiveFlg); }

private:

	enum
	{
		ActiveFlg,
		PermissionFlg
	};

private:

	static constexpr short		m_totalEachFlg = 2;
	std::bitset<m_totalEachFlg>	m_bitsEachFlg;
};
