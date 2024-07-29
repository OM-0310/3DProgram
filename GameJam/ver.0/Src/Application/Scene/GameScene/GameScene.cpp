#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Player/Player.h"
#include "../../GameObject/FoodBlock/MeetBlock/MeetBlock.h"
#include "../../GameObject/FoodBlock/VegetableBlock/VegetableBlock.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"

#include "../../GameObject/Food/Meet/Meet.h"

void GameScene::ObjectInit(std::atomic<bool>& done)
{
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	player->Init();
	m_objList.push_back(player);
	m_wpPlayer = player;

	std::shared_ptr<MeetBlock> meet_B;
	meet_B = std::make_shared<MeetBlock>();
	meet_B->Init();
	m_objList.push_back(meet_B);

	std::shared_ptr<VegetableBlock> vegeta_B;
	vegeta_B = std::make_shared<VegetableBlock>();
	vegeta_B->Init();
	m_objList.push_back(vegeta_B);

	player->SetOwner(this);
	player->SetMeetBlock(meet_B);
	player->SetVegetableBlock(vegeta_B);

	done = true;
}

void GameScene::CameraInit(std::atomic<bool>& done)
{
	std::shared_ptr<TPSCamera> camera;
	camera = std::make_shared<TPSCamera>();
	camera->Init();
	m_objList.push_back(camera);

	done = true;
}

void GameScene::Event()
{
}

void GameScene::Init()
{
	std::atomic<bool> objDone;
	std::thread objTh(&GameScene::ObjectInit, this, std::ref(objDone));

	std::atomic<bool> camDone;
	std::thread camTh(&GameScene::CameraInit, this, std::ref(camDone));

	objTh.join();
	camTh.join();
}
