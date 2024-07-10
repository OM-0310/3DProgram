#pragma once

#include"../BaseScene/BaseScene.h"

class GameScene : public BaseScene
{
public :

	GameScene()		{ Init(); }
	~GameScene()	{}

private:

	// 少数第n位で四捨五入する
	void round_n(float& number, int n)
	{
		number = number * pow(10, n - 1);
		number = round(number);
		number /= pow(10, n - 1);
	}

	void Event() override;
	void Init() override;
};
