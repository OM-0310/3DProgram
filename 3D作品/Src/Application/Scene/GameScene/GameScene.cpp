#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"

#include "../../GameObject/Sky/Sky.h"
#include "../../GameObject/Terrains/Ground/Ground.h"
#include "../../GameObject/Terrains/Building/Building.h"
#include "../../GameObject/Terrains/Building/Building_Main/Building_Main.h"
#include "../../GameObject/Terrains/Building/Building_Roof/Building_Roof.h"

#include "../../GameObject/Gimmicks/Door/Door_1.h"
#include "../../GameObject/Gimmicks/Door/Door_2.h"
#include "../../GameObject/Gimmicks/Door/Door_3.h"
#include "../../GameObject/Gimmicks/LockedDoor/LockedDoor.h"

#include "../../GameObject/Item/SecretFile/SecretFile.h"
#include "../../GameObject/Item/CardKey/CardKey.h"
#include "../../GameObject/Goal/Goal.h"

#include "../../GameObject/Characters/Player/Player.h"
#include "../../GameObject/Characters/Player/Player_UpperBody/Player_UpperBody.h"
#include "../../GameObject/Characters/Player/Player_LowerBody/Player_LowerBody.h"
#include "../../GameObject/Characters/Player/Player_Disarm/Player_Disarm.h"
#include "../../GameObject/Characters/Player/Player_Disarm_Pistol/Player_Disarm_Pistol.h"
#include "../../GameObject/Characters/Player/Player_Ready_Pistol/Player_Ready_Pistol.h"

#include "../../GameObject/Characters/Enemy/Enemy.h"

#include "../../GameObject/Weapon/Pistol/Pistol_Disarm/Pistol_Disarm.h"
#include "../../GameObject/Weapon/Pistol/Pistol_Ready/Pistol_Ready.h""
#include "../../GameObject/Weapon/AssaultRifle/AssaultRifle.h"
#include "../../GameObject/Characters/CharaBase.h"

#include "../../GameObject/UI/Reticle/Reticle.h"
#include "../../GameObject/UI/CardKeyUI/CardKeyUI.h"
#include "../../GameObject/UI/SecretFileUI/SecretFileUI.h"
#include "../../GameObject/UI/PadKeyUI/PadKeyUI.h"

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
	// マルチスレッド
	//=================================================================

	std::atomic<bool> stageDone(false);
	std::thread stageTh(&GameScene::StageInit, this, std::ref(stageDone));

	std::atomic<bool> charaDone(false);
	std::thread charaTh(&GameScene::CharaInit, this, std::ref(charaDone));

	stageTh.join();
	charaTh.join();
}

void GameScene::StageInit(std::atomic<bool>& done)
{
	//=================================================================
	// ステージ関係初期化
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

	done = true;
}

void GameScene::CharaInit(std::atomic<bool>& done)
{
	//=================================================================
	// キャラ関係初期化
	//=================================================================

	// プレイヤー初期化
	std::shared_ptr<Player> player;
	player = std::make_shared<Player>();
	player->Init();
	m_objList.push_back(player);
	m_wpPlayer = player;

	// プレイヤー上半身
	std::shared_ptr<Player_UpperBody> player_Up;
	player_Up = std::make_shared<Player_UpperBody>();
	player_Up->Init();
	m_objList.push_back(player_Up);

	// プレイヤー下半身
	std::shared_ptr<Player_LowerBody> player_Low;
	player_Low = std::make_shared<Player_LowerBody>();
	player_Low->Init();
	m_objList.push_back(player_Low);

	// プレイヤー(武装解除状態)初期化
	std::shared_ptr<Player_Disarm> player_Disarm;
	player_Disarm = std::make_shared<Player_Disarm>();
	player_Disarm->Init();
	m_objList.push_back(player_Disarm);

	std::shared_ptr<Player_Disarm_Pistol> player_Disarm_Pistol;
	player_Disarm_Pistol = std::make_shared<Player_Disarm_Pistol>();
	player_Disarm_Pistol->Init();
	m_objList.push_back(player_Disarm_Pistol);

	std::shared_ptr<Player_Ready_Pistol> player_Ready_Pistol;
	player_Ready_Pistol = std::make_shared<Player_Ready_Pistol>();
	player_Ready_Pistol->Init();
	m_objList.push_back(player_Ready_Pistol);

	// 敵初期化
	//std::shared_ptr<Enemy> enemy;
	//enemy = std::make_shared<Enemy>();
	//enemy->Init();
	//m_objList.push_back(enemy);

	//=================================================================
	// 武器関係
	//=================================================================

	// 銃(ピストル)
	//std::shared_ptr<Pistol_Disarm> pistol_Disarm;
	//pistol_Disarm = std::make_shared<Pistol_Disarm>();
	//pistol_Disarm->Init();
	//m_objList.push_back(pistol_Disarm);

	//std::shared_ptr<Pistol_Ready> pistol_Ready;
	//pistol_Ready = std::make_shared<Pistol_Ready>();
	//pistol_Ready->Init();
	//m_objList.push_back(pistol_Ready);


	//// 銃(アサルトライフル)
	//std::shared_ptr<AssaultRifle> assault;
	//assault = std::make_shared<AssaultRifle>();
	//assault->Init();
	//m_objList.push_back(assault);

	//=================================================================
	// アイテム関係
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
	// ギミック関係
	//=================================================================

	// ドア初期化
	std::shared_ptr<Door_1> door_1;
	door_1 = std::make_shared<Door_1>();
	door_1->Init();
	m_objList.push_back(door_1);

	std::shared_ptr<Door_2> door_2;
	door_2 = std::make_shared<Door_2>();
	door_2->Init();
	m_objList.push_back(door_2);

	std::shared_ptr<Door_3> door_3;
	door_3 = std::make_shared<Door_3>();
	door_3->Init();
	m_objList.push_back(door_3);

	// 鍵のかかったドア初期化
	std::shared_ptr<LockedDoor> lockedDoor;
	lockedDoor = std::make_shared<LockedDoor>();
	lockedDoor->Init();
	m_objList.push_back(lockedDoor);
	
	//=================================================================
	// カメラ初期化
	//=================================================================

	// カメラ初期化
	std::shared_ptr<TPSCamera> camera;
	camera = std::make_shared<TPSCamera>();
	camera->Init();
	m_objList.push_back(camera);
	
	//// UI関連初期化 //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

	//=================================================================
	// レティクル初期化
	//=================================================================

	std::shared_ptr<Reticle> reticle;
	reticle = std::make_shared<Reticle>();
	reticle->Init();
	m_objList.push_back(reticle);

	//=================================================================
	// カードキー取得UI初期化
	//=================================================================
	std::shared_ptr<CardKeyUI> cardUI;
	cardUI = std::make_shared<CardKeyUI>();
	cardUI->Init();
	m_objList.push_back(cardUI);

	//=================================================================
	// 機密ファイル取得UI初期化
	//=================================================================
	std::shared_ptr<SecretFileUI> fileUI;
	fileUI = std::make_shared<SecretFileUI>();
	fileUI->Init();
	m_objList.push_back(fileUI);

	//=================================================================
	// キーロック解除UI初期化
	//=================================================================
	std::shared_ptr<PadKeyUI> keyUI;
	keyUI = std::make_shared<PadKeyUI>();
	keyUI->Init();
	m_objList.push_back(keyUI);

	//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //

	player->SetCamera(camera);
	player->SetReticle(reticle);
	player->SetDoor(lockedDoor);
	player->SetCardKey(card);
	player->SetSecretFile(file);
	player->SetGoal(goal);
	//player->SetPistolReady(pistol_Ready);
	player->SetPlayerUpperBody(player_Up);
	player->SetPlayerLowerBody(player_Low);
	player->SetPlayerDisarm(player_Disarm);
	player->SetPlayerDisarmPistol(player_Disarm_Pistol);
	player->SetPlayerReadyPistol(player_Ready_Pistol);
	player_Up->SetPlayer(player);
	player_Low->SetPlayer(player);
	player_Disarm->SetPlayer(player);
	player_Disarm_Pistol->SetPlayer(player);
	player_Ready_Pistol->SetPlayer(player);

	door_1->SetPlayer(player);
	door_2->SetPlayer(player);
	door_3->SetPlayer(player);
	lockedDoor->SetPlayer(player);
	card->SetPlayer(player);

	//player->SetWeapon(assault);
	//pistol_Disarm->SetPlayer(player);
	//pistol_Ready->SetPlayer(player);
	//assault->SetChara(player);

	cardUI->SetCardKey(card);
	fileUI->SetSecretFile(file);
	keyUI->SetLockedDoor(lockedDoor);

	camera->SetPlayer(player);

	done = true;
}
