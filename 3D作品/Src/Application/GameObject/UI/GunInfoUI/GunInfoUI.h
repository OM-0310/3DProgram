#pragma once

#include "../UIBase.h"

class GunInfoUI : public UIBase
{
public:

	GunInfoUI		()				{}
	~GunInfoUI		()	override	{}

	void Init		()	override;
	void DrawSprite	()	override;

private:
};
