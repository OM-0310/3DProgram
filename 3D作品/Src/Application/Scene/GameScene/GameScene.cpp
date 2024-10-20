#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"

#include "../../GameObject/Sky/Sky.h"
#include "../../GameObject/Terrains/Ground/Ground.h"
#include "../../GameObject/Terrains/Building/Building.h"
#include "../../GameObject/Terrains/Building/Building_Main/Building_Main.h"
#include "../../GameObject/Terrains/Building/Building_Roof/Building_Roof.h"

#include "../../GameObject/Gimmicks/Door/Door.h"

#include "../../GameObject/SecretFile/SecretFile.h"
#include "../../GameObject/CardKey/CardKey.h"
#include "../../GameObject/Goal/Goal.h"

#include "../../GameObject/Characters/Player/Player.h"
#include "../../GameObject/Characters/Enemy/Enemy.h"

#include "../../GameObject/Weapon/Pistol/Pistol_Disarm/Pistol_Disarm.h"
#include "../../GameObject/Weapon/Pistol/Pistol_Ready/Pistol_Ready.h"
#include "../../GameObject/Weapon/AssaultRifle/AssaultRifle.h"
#include "../../GameObject/Characters/CharaBase.h"

#include "../../GameObject/UI/Reticle/Reticle.h"
#include "../../GameObject/UI/CardKeyUI/CardKeyUI.h"

void GameScene::Event()
{
	std::shared_ptr<Goal> spGoal = m_wpGoal.lock();
	if (spGoal)
	{
		if (spGoal->GetClearFlg())
		{
			SceneManager::Instance().SetNextScene
			(
				SceneManager::SceneType::Result
			);
		}
	}
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

	// 建物　メイン
	std::shared_ptr<Building_Main> build_main;
	build_main = std::make_shared<Building_Main>();
	build_main->Init();
	m_objList.push_back(build_main);

	// 建物　屋根
	std::shared_ptr<Building_Roof> build_roof;
	build_roof = std::make_shared<Building_Roof>();
	build_roof->Init();
	m_objList.push_back(build_roof);

	//std::shared_ptr<Building> build;
	//build = std::make_shared<Building>();
	//build->Init();
	//m_objList.push_back(build);

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
	//std::shared_ptr<Enemy> enemy;
	//enemy = std::make_shared<Enemy>();
	//enemy->Init();
	//m_objList.push_back(enemy);

	//=================================================================
	// キャラ関係初期化・・・ここまで
	//=================================================================

	//=================================================================
	// 武器関係・・・ここから
	//=================================================================

	// 銃(ピストル)
	std::shared_ptr<Pistol_Disarm> pistol_Disarm;
	pistol_Disarm = std::make_shared<Pistol_Disarm>();
	pistol_Disarm->Init();
	m_objList.push_back(pistol_Disarm);

	std::shared_ptr<Pistol_Ready> pistol_Ready;
	pistol_Ready = std::make_shared<Pistol_Ready>();
	pistol_Ready->Init();
	m_objList.push_back(pistol_Ready);


	//// 銃(アサルトライフル)
	//std::shared_ptr<AssaultRifle> assault;
	//assault = std::make_shared<AssaultRifle>();
	//assault->Init();
	//m_objList.push_back(assault);

	//=================================================================
	// 武器関係・・・ここまで
	//=================================================================

	//=================================================================
	// アイテム関係・・・ここから
	//=================================================================

	// カードキー初期化
	std::shared_ptr<CardKey> card;
	card = std::make_shared<CardKey>();
	card->Init();
	m_objList.push_back(card);

	// 機密データ初期化
	std::shared_ptr<SecretFile> file;
	file = std::make_shared<SecretFile>();
	file->Init();
	m_objList.push_back(file);

	// ゴール
	std::shared_ptr<Goal> goal;
	goal = std::make_shared<Goal>();
	goal->Init();
	m_objList.push_back(goal);
	m_wpGoal = goal;

	//=================================================================
	// アイテム関係・・・ここまで
	//=================================================================

	//=================================================================
	// ギミック関係・・・ここから
	//=================================================================

	// ドア初期化
	std::shared_ptr<Door> door;
	door = std::make_shared<Door>();
	door->Init();
	m_objList.push_back(door);

	//=================================================================
	// ギミック関係・・・ここまで
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

	//=================================================================
	// カードキーUI初期化・・・ここから
	//=================================================================

	std::shared_ptr<CardKeyUI> cardUI;
	cardUI = std::make_shared<CardKeyUI>();
	cardUI->Init();
	m_objList.push_back(cardUI);

	//=================================================================
	// カードキーUI初期化・・・ここまで
	//=================================================================

	//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //

	player->SetCamera(camera);
	player->SetReticle(reticle);
	player->SetDoor(door);
	player->SetCardKey(card);
	player->SetSecretFile(file);
	player->SetGoal(goal);
	player->SetPistolDisarm(pistol_Disarm);
	player->SetPistolReady(pistol_Ready);
	//enemy->SetPlayer(player);

	door->SetPlayer(player);
	card->SetPlayer(player);

	//player->SetWeapon(assault);
	pistol_Disarm->SetPlayer(player);
	pistol_Ready->SetPlayer(player);
	//assault->SetChara(player);

	cardUI->SetCardKey(card);

	camera->SetPlayer(player);

	done = true;
}
