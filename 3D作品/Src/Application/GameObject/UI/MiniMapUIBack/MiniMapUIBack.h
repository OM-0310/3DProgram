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

	void Open		(const bool _active) { m_active = _active; }

	const bool& GetActive()const { return m_active; }

private:

	bool m_active = false;
};
