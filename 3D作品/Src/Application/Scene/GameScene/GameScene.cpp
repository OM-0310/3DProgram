#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"

#include "../../GameObject/Characters/Player/Player.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	// ステージ初期化

	// プレイヤー初期化
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	player->Init();
	m_objList.push_back(player);

	// カメラ初期化
	std::shared_ptr<TPSCamera> camera;
	camera = std::make_shared<TPSCamera>();
	camera->Init();
	m_objList.push_back(camera);
}
