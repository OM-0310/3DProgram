#pragma once

#include"../BaseScene/BaseScene.h"

class GameOver_Back;
class SelectTitle;
class SelectRetry;

class GameOverScene : public BaseScene
{
public:

	GameOverScene	() { Init(); }
	~GameOverScene	() {}

private:

	void Event	() override;
	void Init	() override;

private:

	std::weak_ptr<GameOver_Back>	m_wpBack;
	std::weak_ptr<SelectTitle>		m_wpTitle;
	std::weak_ptr<SelectRetry>		m_wpRetry;
};
