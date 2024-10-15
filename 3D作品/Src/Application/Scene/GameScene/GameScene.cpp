#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"

#include "../../GameObject/Sky/Sky.h"
#include "../../GameObject/Terrains/Ground/Ground.h"
#include "../../GameObject/Terrains/Building/Building.h"

#include "../../GameObject/Characters/Player/Player.h"
#include "../../GameObject/Characters/Enemy/Enemy.h"

#include "../../GameObject/Weapon/Pistol/Pistol.h"
#include "../../GameObject/Weapon/AssaultRifle/AssaultRifle.h"
#include "../../GameObject/Characters/CharaBase.h"

#include "../../GameObject/UI/Reticle/Reticle.h"

void GameScene::Event()
{
}

void GameScene::Init()
{
	ShowCursor(false);

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

	// 空
	std::shared_ptr<Sky> sky;
	sky = std::make_shared<Sky>();
	sky->Init();
	m_objList.push_back(sky);

	// 地形　地面初期化
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	ground->Init();
	m_objList.push_back(ground);

	// 建物
	std::shared_ptr<Building> build;
	build = std::make_shared<Building>();
	build->Init();
	m_objList.push_back(build);

	//=================================================================
	// ステージ関係初期化・・・ここまで
	//=================================================================

	done = true;
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

	// 敵初期化
	std::shared_ptr<Enemy> enemy;
	enemy = std::make_shared<Enemy>();
	enemy->Init();
	m_objList.push_back(enemy);

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
	
	//// UI関連初期化 //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

	//=================================================================
	// レティクル初期化・・・ここから
	//=================================================================

	std::shared_ptr<Reticle> reticle;
	reticle = std::make_shared<Reticle>();
	reticle->Init();
	m_objList.push_back(reticle);

	//=================================================================
	// レティクル初期化・・・ここまで
	//=================================================================

	//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //

	player->SetCamera(camera);
	player->SetReticle(reticle);
	enemy->SetPlayer(player);
	//player->SetWeapon(pistol);
	//player->SetWeapon(assault);
	//pistol->SetChara(player);
	//assault->SetChara(player);
	camera->SetPlayer(player);

	done = true;
}
