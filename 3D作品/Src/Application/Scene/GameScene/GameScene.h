#pragma once

#include"../BaseScene/BaseScene.h"

class Player;
class TPSCamera;

class GameScene : public BaseScene
{
public :

	GameScene		() { Init(); }
	~GameScene		() {}

private:

	void Event		() override;
	void Init		() override;

	void StageInit	(std::atomic<bool>& done);
	void CharaInit	(std::atomic<bool>& done);

private:

	std::weak_ptr<Player>	m_wpPlayer;
	std::weak_ptr<TPSCamera>m_wpTpsCamera;
};
