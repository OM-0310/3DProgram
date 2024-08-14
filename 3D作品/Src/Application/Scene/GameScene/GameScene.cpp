#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"

#include "../../GameObject/Terrains/Ground/Ground.h"
#include "../../GameObject/Characters/Player/Player.h"
#include "../../GameObject/Weapon/Pistol/Pistol.h"
#include "../../GameObject/Weapon/AssaultRifle/AssaultRifle.h"
#include "../../GameObject/Characters/CharaBase.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	//=================================================================
	// マルチスレッド・・・ここから
	//=================================================================

	std::atomic<bool> stageDone(false);
	std::thread stageTh(&GameScene::StageInit, this, std::ref(stageDone));

	std::atomic<bool> charaDone(false);
	std::thread charaTh(&GameScene::CharaInit, this, std::ref(charaDone));

	stageTh.join();
	charaTh.join();

	//=================================================================
	// マルチスレッド・・・ここまで
	//=================================================================
}

void GameScene::StageInit(std::atomic<bool>& done)
{
	//=================================================================
	// ステージ関係初期化・・・ここから
	//=================================================================

	// 地形　地面初期化
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	ground->Init();
	m_objList.push_back(ground);

	done = true;

	//=================================================================
	// ステージ関係初期化・・・ここまで
	//=================================================================
}

void GameScene::CharaInit(std::atomic<bool>& done)
{
	//=================================================================
	// キャラ関係初期化・・・ここから
	//=================================================================

	// プレイヤー初期化
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	player->Init();
	m_objList.push_back(player);
	m_wpPlayer = player;

	//=================================================================
	// キャラ関係初期化・・・ここまで
	//=================================================================

	//=================================================================
	// 武器関係・・・ここから
	//=================================================================

	//// 銃(ピストル)
	//std::shared_ptr<Pistol> pistol;
	//pistol = std::make_shared<Pistol>();
	//pistol->Init();
	//m_objList.push_back(pistol);

	//// 銃(アサルトライフル)
	//std::shared_ptr<AssaultRifle> assault;
	//assault = std::make_shared<AssaultRifle>();
	//assault->Init();
	//m_objList.push_back(assault);

	//=================================================================
	// 武器関係・・・ここまで
	//=================================================================
	
	//=================================================================
	// カメラ初期化・・・ここから
	//=================================================================

	// カメラ初期化
	std::shared_ptr<TPSCamera> camera;
	camera = std::make_shared<TPSCamera>();
	camera->Init();
	m_objList.push_back(camera);

	//=================================================================
	// カメラ初期化・・・ここまで
	//=================================================================

	player->SetCamera(camera);
	//player->SetWeapon(pistol);
	//player->SetWeapon(assault);
	//pistol->SetChara(player);
	//assault->SetChara(player);
	camera->SetPlayer(player);

	done = true;
}
