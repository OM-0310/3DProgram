#pragma once

#include "../BaseScene/BaseScene.h"

class Result_Back;

class ResultScene : public BaseScene 
{
public:

	ResultScene	()				{ Init(); }
	~ResultScene()	override	{}

private:

	void Event	()	override;
	void Init	()	override;

	std::weak_ptr<Result_Back> m_wpBack;
};
