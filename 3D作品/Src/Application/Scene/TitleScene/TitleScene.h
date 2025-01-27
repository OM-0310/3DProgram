#pragma once

#include"../BaseScene/BaseScene.h"

class Title_Back;
class Helicopter;

class TitleScene : public BaseScene
{
public :

	TitleScene() { Init(); }
	~TitleScene() {}

private :

	void Event() override;
	void Init() override;

	void TitleInit	(std::atomic<bool>& done);
	void StageInit	(std::atomic<bool>& done);
	void CharaInit	(std::atomic<bool>& done);
	void BackInit	(std::atomic<bool>& done);

	std::weak_ptr<Title_Back> m_wpBack;
	std::weak_ptr<Helicopter> m_wpHelicopter;
};
