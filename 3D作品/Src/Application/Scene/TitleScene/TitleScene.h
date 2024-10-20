#pragma once

#include"../BaseScene/BaseScene.h"

class TitleScene : public BaseScene
{
public :

	TitleScene() { Init(); }
	~TitleScene() {}

private :

	void Event() override;
	void Init() override;

	void StageInit(std::atomic<bool>& done);

};
