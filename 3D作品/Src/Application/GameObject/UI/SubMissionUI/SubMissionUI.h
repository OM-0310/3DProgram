#pragma once

#include "../UIBase.h"

class SubMissionUI : public UIBase
{
public:

	SubMissionUI	()				{}
	~SubMissionUI	()	override	{}

	void Init		()	override;
	void DrawSprite	()	override;

	void Permission	() { m_permissionFlg = true; }
	void Open(const bool _active) 
	{ 
		if (!m_permissionFlg) { return; }
		m_active = _active;
	}

	const bool& GetActive() const { return m_active; }

private:

	bool m_permissionFlg	= false;
	bool m_active			= false;
};
