#pragma once

#include"../BaseScene/BaseScene.h"

class Player;
class Wild_Zombie;
class Zombie_Woman;

class GameScene : public BaseScene
{
public :

	GameScene() { Init(); }
	~GameScene() {}

	std::shared_ptr<Player> GetPlayer();
	std::shared_ptr<Wild_Zombie> GetWild_Zombie();
	std::shared_ptr<Zombie_Woman>GetZombie_Woman();

private:

	void Event() override;
	void Init() override;

	std::weak_ptr<Player>		m_wpPlayer;
	std::weak_ptr<Wild_Zombie>	m_wpw_Zombie;
	std::weak_ptr<Zombie_Woman> m_wpZombie_w;
};
