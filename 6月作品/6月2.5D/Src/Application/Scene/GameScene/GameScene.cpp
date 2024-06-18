#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object/Stage/Stage.h"
#include "../../Object/Chara/Player/Player.h"
#include "../../Object/Chara/Enemy/Wild_Zombie/Wild_Zombie.h"
#include "../../Object/Chara/Enemy/Zombie_Woman/Zombie_Woman.h"

std::shared_ptr<Player> GameScene::GetPlayer()
{
	std::shared_ptr<Player> player;
	if (!m_wpPlayer.expired())
	{
		player = m_wpPlayer.lock();
	}
	return player;
}

std::shared_ptr<Wild_Zombie> GameScene::GetWild_Zombie()
{
	std::shared_ptr<Wild_Zombie> w_Zombie;
	if (!m_wpw_Zombie.expired())
	{
		w_Zombie = m_wpw_Zombie.lock();
	}
	return w_Zombie;
}

std::shared_ptr<Zombie_Woman> GameScene::GetZombie_Woman()
{
	std::shared_ptr<Zombie_Woman> Zombie_w;
	if (!m_wpZombie_w.expired())
	{
		Zombie_w = m_wpZombie_w.lock();
	}
	return Zombie_w;
}

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}

	Math::Vector3 playerPos;
	if (m_wpPlayer.expired() == false)
	{
		playerPos = m_wpPlayer.lock()->GetPos();
	}

	Math::Matrix transMat;
	Math::Vector3 cameraPos = { 2.0f,1.5f,-3.0f };
	//Math::Vector3 cameraPos = { 0.0f,0.0f,0.0f };
	transMat = Math::Matrix::CreateTranslation(cameraPos + playerPos);
	m_camera->SetCameraMatrix(transMat);
}

void GameScene::Init()
{
	// ==カメラ=========================================
	m_camera = std::make_unique<KdCamera>();

	// ==地面===========================================
	std::shared_ptr<Stage> stage;
	stage = std::make_shared<Stage>();
	stage->Init();
	m_objList.push_back(stage);

	// ==プレイヤー======================================
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	player->Init();
	player->SetOwner(this);
	m_objList.push_back(player);
	m_wpPlayer = player;

	// ==敵(ゾンビ)======================================
	std::shared_ptr<Wild_Zombie> w_zombie;
	w_zombie = std::make_shared<Wild_Zombie>();
	w_zombie->Init();
	m_objList.push_back(w_zombie);
	m_wpw_Zombie = w_zombie;
	w_zombie->SetPlayer(m_wpPlayer);
	player->SetWild_Zombie(m_wpw_Zombie);

	// ==敵(女ゾンビ)====================================
	std::shared_ptr<Zombie_Woman> zombie_w;
	zombie_w = std::make_shared<Zombie_Woman>();
	zombie_w->Init();
	zombie_w->SetOwner(this);
	m_objList.push_back(zombie_w);
	m_wpZombie_w = zombie_w;
	zombie_w->SetPlayer(m_wpPlayer);
}
