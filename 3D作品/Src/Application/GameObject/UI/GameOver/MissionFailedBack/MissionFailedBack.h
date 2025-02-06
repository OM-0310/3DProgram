#pragma once

#include "../../UIBase.h"

class MissionFailedUIBack : public UIBase
{
public:

	MissionFailedUIBack	()				{}
	~MissionFailedUIBack()	override	{}

	void Init			()	override;
	void DrawSprite		()	override;

private:
};
