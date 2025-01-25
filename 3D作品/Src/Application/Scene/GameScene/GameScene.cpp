#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"

#include "../../GameObject/Sky/Sky.h"
#include "../../GameObject/Terrains/Ground/Ground_UP/Ground_UP.h"
#include "../../GameObject/Terrains/Ground/Ground_Bottom/Ground_Bottom.h"
#include "../../GameObject/Terrains/RockWall/RockWall.h"
#include "../../GameObject/Terrains/Building/Building_Main/Building_Main.h"
#include "../../GameObject/Terrains/Building/Building_Roof/Building_Roof.h"
#include "../../GameObject/Terrains/ArmoredCar/ArmoredCar.h"
#include "../../GameObject/Terrains/Grass/Grass.h"

#include "../../GameObject/Gimmicks/Door/Door_1.h"
#include "../../GameObject/Gimmicks/Door/Door_2.h"
#include "../../GameObject/Gimmicks/Door/Door_3.h"
#include "../../GameObject/Gimmicks/LockedDoor/LockedDoor.h"

#include "../../GameObject/Item/SecretFile/SecretFile.h"
#include "../../GameObject/Item/CardKey/CardKey.h"
#include "../../GameObject/Goal/Goal.h"

#include "../../GameObject/Characters/Player/Player.h"
#include "../../GameObject/Characters/Player/Player_Main/Player_Main.h"
#include "../../GameObject/Characters/Player/Player_Disarm/Player_Disarm.h"
#include "../../GameObject/Characters/Player/Player_Disarm_Pistol/Player_Disarm_Pistol.h"
#include "../../GameObject/Characters/Player/Player_Ready_Pistol/Player_Ready_Pistol.h"

#include "../../GameObject/Characters/Enemy/Enemy_1/Enemy_1.h"
#include "../../GameObject/Characters/Enemy/Enemy_1/Enemy_Main_1/Enemy_Main_1.h"
#include "../../GameObject/Characters/Enemy/Enemy_1/Enemy_Gun_1/Enemy_Gun_1.h"
#include "../../GameObject/Characters/Enemy/Enemy_1/Enemy_Gun_NoMagazine_1/Enemy_Gun_NoMagazine_1.h"
#include "../../GameObject/Characters/Enemy/Enemy_1/Enemy_Magazine_1/Enemy_Magazine_1.h"

#include "../../GameObject/Characters/Enemy/Enemy_2/Enemy_2.h"
#include "../../GameObject/Characters/Enemy/Enemy_2/Enemy_Main_2/Enemy_Main_2.h"
#include "../../GameObject/Characters/Enemy/Enemy_2/Enemy_Gun_2/Enemy_Gun_2.h"
#include "../../GameObject/Characters/Enemy/Enemy_2/Enemy_Gun_NoMagazine_2/Enemy_Gun_NoMagazine_2.h"
#include "../../GameObject/Characters/Enemy/Enemy_2/Enemy_Magazine_2/Enemy_Magazine_2.h"

#include "../../GameObject/Characters/Enemy/Enemy_3/Enemy_3.h"
#include "../../GameObject/Characters/Enemy/Enemy_3/Enemy_Main_3/Enemy_Main_3.h"
#include "../../GameObject/Characters/Enemy/Enemy_3/Enemy_Gun_3/Enemy_Gun_3.h"
#include "../../GameObject/Characters/Enemy/Enemy_3/Enemy_Gun_NoMagazine_3/Enemy_Gun_NoMagazine_3.h"
#include "../../GameObject/Characters/Enemy/Enemy_3/Enemy_Magazine_3/Enemy_Magazine_3.h"

#include "../../GameObject/Characters/CharaBase.h"

#include "../../GameObject/UI/GunInfoUI/GunInfoUI.h"
#include "../../GameObject/UI/GunInfoUIBack/GunInfoUIBack.h"
#include "../../GameObject/UI/BulletNumUI/BulletNumUI.h"
#include "../../GameObject/UI/Reticle/Reticle.h"
#include "../../GameObject/UI/CardKeyUI/CardKeyUI.h"
#include "../../GameObject/UI/KillUI/KillUI.h"
#include "../../GameObject/UI/InterrogationUI/InterrogationUI.h"
#include "../../GameObject/UI/SecretFileUI/SecretFileUI.h"
#include "../../GameObject/UI/RestraintUI/RestraintUI.h"
#include "../../GameObject/UI/HPBarUI/HPBarUI.h"
#include "../../GameObject/UI/HPBarUIBack/HPBarUIBack.h"
#include "../../GameObject/UI/PadKeyUI/PadKeyUI.h"
#include "../../GameObject/UI/MiniMapUIBack/MiniMapUIBack.h"
#include "../../GameObject/UI/MiniMapUI/MiniMapUI.h"
#include "../../GameObject/UI/MainMissionUI/MainMissionUI.h"
#include "../../GameObject/UI/CurrentLocation/CurrentLocation.h"
#include "../../GameObject/UI/CardKeyLocation/CardKeyLocation.h"
#include "../../GameObject/UI/SecretFileLocation/SecretFileLocation.h"

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
	std::mutex mtx;
	std::condition_variable cv;
	std::thread stageTh(&GameScene::StageInit, this, std::ref(stageDone), std::ref(cv), std::ref(mtx));

	std::atomic<bool> charaDone(false);
	std::thread charaTh(&GameScene::CharaInit, this, std::ref(charaDone), std::ref(stageDone), std::ref(cv), std::ref(mtx));

	std::atomic<bool> grassDone(false);
	std::thread grassTh(&GameScene::GrassInit, this, std::ref(grassDone), std::ref(stageDone), std::ref(cv), std::ref(mtx));

	stageTh.join();
	grassTh.join();
	charaTh.join();
}

void GameScene::StageInit(
	std::atomic<bool>& done,
	std::condition_variable& cv,
	std::mutex& mtx)
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
	std::shared_ptr<Ground_UP> ground_Up;
	ground_Up = std::make_shared<Ground_UP>();
	ground_Up->Init();
	m_objList.push_back(ground_Up);

	std::shared_ptr<Ground_Bottom> ground_Bottom;
	ground_Bottom = std::make_shared<Ground_Bottom>();
	ground_Bottom->Init();
	m_objList.push_back(ground_Bottom);

	// 岩初期化
	std::shared_ptr<RockWall> rock;
	rock = std::make_shared<RockWall>();
	rock->Init();
	m_objList.push_back(rock);

	//std::shared_ptr<Ground> ground;
	//ground = std::make_shared<Ground>();
	//ground->Init();
	//m_objList.push_back(ground);

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

	// 装甲車&戦車
	std::shared_ptr<ArmoredCar> car;
	car = std::make_shared<ArmoredCar>();
	car->Init();
	m_objList.push_back(car);

	{
		// 初期化完了を通知
		std::lock_guard<std::mutex> lock(mtx);
		done = true;
	}

	// 待機中のスレッドに通知
	cv.notify_all();
}

void GameScene::GrassInit(
	std::atomic<bool>& done,	std::atomic<bool>& stageDone,
	std::condition_variable& cv,std::mutex& mtx)
{
	// ステージ初期化完了を待つ
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&stageDone]() {return stageDone.load(); });
	}

	//=================================================================
	// 草初期化
	//=================================================================

	//std::shared_ptr<Grass> grass;
	//for (int i = 0; i < 4; i++)
	//{
	//	grass = std::make_shared<Grass>();
	//	grass->Init();
	//	grass->SetPos({ 0.0f,-1.0f,-30.0f * i });
	//	m_objList.push_back(grass);
	//}

	done = true;
}

void GameScene::CharaInit(
	std::atomic<bool>& done, std::atomic<bool>& stageDone,
	std::condition_variable& cv, std::mutex& mtx)
{
	// ステージ初期化完了を待つ
	{
		std::unique_lock<std::mutex> lock(mtx);
		cv.wait(lock, [&stageDone]() {return stageDone.load(); });
	}

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
	std::shared_ptr<Player_Main> player_Main;
	player_Main = std::make_shared<Player_Main>();
	player_Main->Init();
	m_objList.push_back(player_Main);

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
	std::shared_ptr<Enemy_1>				enemy_1;
	std::shared_ptr<Enemy_Main_1>			enemy_Main_1;
	std::shared_ptr<Enemy_Gun_1>			enemy_Gun_1;
	std::shared_ptr<Enemy_Gun_NoMagazine_1>	enemy_Gun_NoMagazine_1;
	std::shared_ptr<Enemy_Magazine_1>		enemy_Magazine_1;

	// 敵管理初期化
	enemy_1 = std::make_shared<Enemy_1>();
	enemy_1->Init();
	m_objList.push_back(enemy_1);

	// 敵メイン部分初期化
	enemy_Main_1 = std::make_shared<Enemy_Main_1>();
	enemy_Main_1->Init();
	m_objList.push_back(enemy_Main_1);

	// 敵銃(マガジンあり)
	enemy_Gun_1 = std::make_shared<Enemy_Gun_1>();
	enemy_Gun_1->Init();
	m_objList.push_back(enemy_Gun_1);

	// 敵銃(マガジン無し)
	enemy_Gun_NoMagazine_1 = std::make_shared<Enemy_Gun_NoMagazine_1>();
	enemy_Gun_NoMagazine_1->Init();
	m_objList.push_back(enemy_Gun_NoMagazine_1);

	// 敵マガジン本体
	enemy_Magazine_1 = std::make_shared<Enemy_Magazine_1>();
	enemy_Magazine_1->Init();
	m_objList.push_back(enemy_Magazine_1);

	enemy_1->SetPlayer(player);
	enemy_1->SetEnemy_Main(enemy_Main_1);
	enemy_1->SetEnemy_Gun(enemy_Gun_1);
	enemy_1->SetEnemy_Gun_NoMagazine(enemy_Gun_NoMagazine_1);
	enemy_1->SetEnemy_Magazine(enemy_Magazine_1);
	enemy_Main_1->SetEnemy(enemy_1);
	enemy_Gun_1->SetEnemy(enemy_1);
	enemy_Gun_NoMagazine_1->SetEnemy(enemy_1);
	enemy_Magazine_1->SetEnemy(enemy_1);

	std::shared_ptr<Enemy_2>				enemy_2;
	std::shared_ptr<Enemy_Main_2>			enemy_Main_2;
	std::shared_ptr<Enemy_Gun_2>			enemy_Gun_2;
	std::shared_ptr<Enemy_Gun_NoMagazine_2>	enemy_Gun_NoMagazine_2;
	std::shared_ptr<Enemy_Magazine_2>		enemy_Magazine_2;

	// 敵管理初期化
	enemy_2 = std::make_shared<Enemy_2>();
	enemy_2->Init();
	m_objList.push_back(enemy_2);

	// 敵メイン部分初期化
	enemy_Main_2 = std::make_shared<Enemy_Main_2>();
	enemy_Main_2->Init();
	m_objList.push_back(enemy_Main_2);

	// 敵銃(マガジンあり)
	enemy_Gun_2 = std::make_shared<Enemy_Gun_2>();
	enemy_Gun_2->Init();
	m_objList.push_back(enemy_Gun_2);

	// 敵銃(マガジン無し)
	enemy_Gun_NoMagazine_2 = std::make_shared<Enemy_Gun_NoMagazine_2>();
	enemy_Gun_NoMagazine_2->Init();
	m_objList.push_back(enemy_Gun_NoMagazine_2);

	// 敵マガジン本体
	enemy_Magazine_2 = std::make_shared<Enemy_Magazine_2>();
	enemy_Magazine_2->Init();
	m_objList.push_back(enemy_Magazine_2);

	enemy_2->SetPlayer(player);
	enemy_2->SetEnemy_Main(enemy_Main_2);
	enemy_2->SetEnemy_Gun(enemy_Gun_2);
	enemy_2->SetEnemy_Gun_NoMagazine(enemy_Gun_NoMagazine_2);
	enemy_2->SetEnemy_Magazine(enemy_Magazine_2);
	enemy_Main_2->SetEnemy(enemy_2);
	enemy_Gun_2->SetEnemy(enemy_2);
	enemy_Gun_NoMagazine_2->SetEnemy(enemy_2);
	enemy_Magazine_2->SetEnemy(enemy_2);

	std::shared_ptr<Enemy_3>				enemy_3;
	std::shared_ptr<Enemy_Main_3>			enemy_Main_3;
	std::shared_ptr<Enemy_Gun_3>			enemy_Gun_3;
	std::shared_ptr<Enemy_Gun_NoMagazine_3>	enemy_Gun_NoMagazine_3;
	std::shared_ptr<Enemy_Magazine_3>		enemy_Magazine_3;

	// 敵管理初期化
	enemy_3 = std::make_shared<Enemy_3>();
	enemy_3->Init();
	m_objList.push_back(enemy_3);

	// 敵メイン部分初期化
	enemy_Main_3 = std::make_shared<Enemy_Main_3>();
	enemy_Main_3->Init();
	m_objList.push_back(enemy_Main_3);

	// 敵銃(マガジンあり)
	enemy_Gun_3 = std::make_shared<Enemy_Gun_3>();
	enemy_Gun_3->Init();
	m_objList.push_back(enemy_Gun_3);

	// 敵銃(マガジン無し)
	enemy_Gun_NoMagazine_3 = std::make_shared<Enemy_Gun_NoMagazine_3>();
	enemy_Gun_NoMagazine_3->Init();
	m_objList.push_back(enemy_Gun_NoMagazine_3);

	// 敵マガジン本体
	enemy_Magazine_3 = std::make_shared<Enemy_Magazine_3>();
	enemy_Magazine_3->Init();
	m_objList.push_back(enemy_Magazine_3);

	enemy_3->SetPlayer(player);
	enemy_3->SetEnemy_Main(enemy_Main_3);
	enemy_3->SetEnemy_Gun(enemy_Gun_3);
	enemy_3->SetEnemy_Gun_NoMagazine(enemy_Gun_NoMagazine_3);
	enemy_3->SetEnemy_Magazine(enemy_Magazine_3);
	enemy_Main_3->SetEnemy(enemy_3);
	enemy_Gun_3->SetEnemy(enemy_3);
	enemy_Gun_NoMagazine_3->SetEnemy(enemy_3);
	enemy_Magazine_3->SetEnemy(enemy_3);

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
	// 銃情報UI初期化
	//=================================================================
	std::shared_ptr<GunInfoUIBack> gunUIBack;
	gunUIBack = std::make_shared<GunInfoUIBack>();
	gunUIBack->Init();
	m_objList.push_back(gunUIBack);

	//=================================================================
	// 銃情報UI初期化
	//=================================================================
	std::shared_ptr<GunInfoUI> gunUI;
	gunUI = std::make_shared<GunInfoUI>();
	gunUI->Init();
	m_objList.push_back(gunUI);

	//=================================================================
	// 残弾数UI初期化
	//=================================================================
	std::shared_ptr<BulletNumUI> bulletNumUI;
	bulletNumUI = std::make_shared<BulletNumUI>();
	bulletNumUI->Init();
	m_objList.push_back(bulletNumUI);

	//=================================================================
	// レティクル初期化
	//=================================================================
	std::shared_ptr<Reticle> reticle;
	reticle = std::make_shared<Reticle>();
	reticle->Init();
	m_objList.push_back(reticle);

	//=================================================================
	// HPバーUI背景初期化
	//=================================================================
	std::shared_ptr<HPBarUIBack> hpBarUIBack;
	hpBarUIBack = std::make_shared<HPBarUIBack>();
	hpBarUIBack->Init();
	m_objList.push_back(hpBarUIBack);

	//=================================================================
	// HPバーUI初期化
	//=================================================================
	std::shared_ptr<HPBarUI> hpBarUI;
	hpBarUI = std::make_shared<HPBarUI>();
	hpBarUI->Init();
	m_objList.push_back(hpBarUI);

	//=================================================================
	// ミニマップUI背景初期化
	//=================================================================
	std::shared_ptr<MiniMapUIBack> mapUIBack;
	mapUIBack = std::make_shared<MiniMapUIBack>();
	mapUIBack->Init();
	m_objList.push_back(mapUIBack);

	//=================================================================
	// ミニマップUI初期化
	//=================================================================
	std::shared_ptr<MiniMapUI> mapUI;
	mapUI = std::make_shared<MiniMapUI>();
	mapUI->Init();
	m_objList.push_back(mapUI);

	//=================================================================
	// メインミッションUI初期化
	//=================================================================
	std::shared_ptr<MainMissionUI> missionUI;
	missionUI = std::make_shared<MainMissionUI>();
	missionUI->Init();
	m_objList.push_back(missionUI);

	//=================================================================
	// 現在地UI初期化
	//=================================================================
	std::shared_ptr<CurrentLocation> currentLocation;
	currentLocation = std::make_shared<CurrentLocation>();
	currentLocation->Init();
	m_objList.push_back(currentLocation);

	//=================================================================
	// カードキー位置UI初期化
	//=================================================================
	std::shared_ptr<CardKeyLocation> cardKeyLocation;
	cardKeyLocation = std::make_shared<CardKeyLocation>();
	cardKeyLocation->Init();
	m_objList.push_back(cardKeyLocation);

	//=================================================================
	// 機密ファイル位置UI初期化
	//=================================================================
	std::shared_ptr<SecretFileLocation> secretFileLocation;
	secretFileLocation = std::make_shared<SecretFileLocation>();
	secretFileLocation->Init();
	m_objList.push_back(secretFileLocation);

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
	// 拘束UI初期化
	//=================================================================
	std::shared_ptr<RestraintUI> restraintUI;
	restraintUI = std::make_shared<RestraintUI>();
	restraintUI->Init();
	m_objList.push_back(restraintUI);

	//=================================================================
	// 殺害UI初期化
	//=================================================================
	std::shared_ptr<KillUI> killUI;
	killUI = std::make_shared<KillUI>();
	killUI->Init();
	m_objList.push_back(killUI);

	//=================================================================
	// 殺害UI初期化
	//=================================================================
	std::shared_ptr<InterrogationUI> interrogationUI;
	interrogationUI = std::make_shared<InterrogationUI>();
	interrogationUI->Init();
	m_objList.push_back(interrogationUI);

	//=================================================================
	// キーロック解除UI初期化
	//=================================================================
	std::shared_ptr<PadKeyUI> keyUI;
	keyUI = std::make_shared<PadKeyUI>();
	keyUI->Init();
	m_objList.push_back(keyUI);

	//// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //// //

	player->SetPlayer_Main(player_Main);
	player->SetPlayer_Disarm(player_Disarm);
	player->SetPlayer_Disarm_Pistol(player_Disarm_Pistol);
	player->SetPlayer_Ready_Pistol(player_Ready_Pistol);

	player->SetEnemy_1(enemy_1);
	player->SetEnemy_2(enemy_2);
	player->SetEnemy_3(enemy_3);

	player->SetCamera(camera);
	player->SetReticle(reticle);
	player->SetMiniMapUI(mapUI);
	player->SetMiniMapUIBack(mapUIBack);
	player->SetMainMissionUI(missionUI);
	player->SetRestraintUI(restraintUI);
	player->SetKillUI(killUI);
	player->SetInterrogationUI(interrogationUI);
	player->SetCurrentLocation(currentLocation);
	player->SetCardKeyLocation(cardKeyLocation);
	player->SetSecretFileLocation(secretFileLocation);
	player->SetDoor(lockedDoor);
	player->SetCardKey(card);
	player->SetSecretFile(file);
	player->SetGoal(goal);

	player_Main->SetPlayer(player);
	player_Disarm->SetPlayer(player);
	player_Disarm_Pistol->SetPlayer(player);
	player_Ready_Pistol->SetPlayer(player);

	lockedDoor->SetPlayer(player);
	reticle->SetTPSCamera(camera);
	bulletNumUI->SetPlayer_Ready_Pistol(player_Ready_Pistol);
	cardUI->SetCardKey(card);
	fileUI->SetSecretFile(file);
	restraintUI->SetPlayer(player);
	restraintUI->SetEnemy_1(enemy_1);
	restraintUI->SetEnemy_2(enemy_2);
	restraintUI->SetEnemy_3(enemy_3);
	killUI->SetPlayer(player);
	killUI->SetInterrogationUI(interrogationUI);
	interrogationUI->SetPlayer(player);
	interrogationUI->SetKillUI(killUI);
	keyUI->SetLockedDoor(lockedDoor);
	currentLocation->SetPlayer(player);
	cardKeyLocation->SetCardKey(card);
	secretFileLocation->SetSecretFile(file);
	hpBarUI->SetPlayer_Main(player_Main);

	camera->SetPlayer(player);

	done = true;
}
