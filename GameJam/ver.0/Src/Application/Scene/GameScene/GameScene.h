#pragma once

#include"../BaseScene/BaseScene.h"

class Player;

class GameScene : public BaseScene
{
public :

	GameScene() { Init(); }
	~GameScene() {}

	std::weak_ptr<Player> GetPlayer() { return m_wpPlayer; }

private:

	void Event() override;
	void Init() override;

	std::weak_ptr<Player> m_wpPlayer;
};
