﻿#pragma once

#include "../BaseScene/BaseScene.h"

class ResultScene : public BaseScene 
{
public:

	ResultScene	()				{}
	~ResultScene()	override	{}

private:

	void Event	()	override;
	void Init	()	override;
};
