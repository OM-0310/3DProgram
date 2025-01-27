#pragma once

#include"../BaseScene/BaseScene.h"

class TPSCamera;
class Player;
class Ground_UP;
class Ground_Bottom;
class RockWall;
class Building_Main;
class Building_Roof;
class ArmoredCar;
class Goal;
class Game_Back;

class GameScene : public BaseScene
{
public :

	GameScene		() { Init(); }
	~GameScene		() {}

private:

	void Event		() override;
	void Init		() override;

	void StageInit	(std::atomic<bool>& done, std::condition_variable& cv, std::mutex& mtx);
	void GrassInit	(std::atomic<bool>& done, std::atomic<bool>& stageDone, std::condition_variable& cv, std::mutex& mtx);
	void CharaInit	(std::atomic<bool>& done, std::atomic<bool>& stageDone, std::condition_variable& cv, std::mutex& mtx);

private:

	std::weak_ptr<TPSCamera>	m_wpTpsCamera;
	std::weak_ptr<Player>		m_wpPlayer;
	std::weak_ptr<Ground_UP>	m_wpGround_Up;
	std::weak_ptr<Ground_Bottom>m_wpGround_Bottom;
	std::weak_ptr<RockWall>		m_wpRock_Wall;
	std::weak_ptr<Building_Main>m_wpBuild_Main;
	std::weak_ptr<Building_Roof>m_wpBuild_Roof;
	std::weak_ptr<ArmoredCar>	m_wpArmoredCar;
	std::weak_ptr<Goal>			m_wpGoal;
	std::weak_ptr<Game_Back>	m_wpBack;
};
