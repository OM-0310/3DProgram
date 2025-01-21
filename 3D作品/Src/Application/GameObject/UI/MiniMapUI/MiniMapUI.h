#pragma once

#include "../UIBase.h"

class MiniMapUI : public UIBase
{
public:

	MiniMapUI		()				{}
	~MiniMapUI		()	override	{}

	void Init		()	override;
	void DrawSprite	()	override;

	void Open		(const bool _active) { m_active = _active; }

	const bool& GetActive() const { return m_active; }

private:

	bool m_active = false;
};
