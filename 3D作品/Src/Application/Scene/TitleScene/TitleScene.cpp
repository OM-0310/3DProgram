#include "TitleScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Camera/TitleCamera/TitleCamera.h"

#include "../../GameObject/Title_Sky/Title_Sky.h"

#include "../../GameObject/Helicopter/Helicopter.h"

#include "../../GameObject/TitleLogo/TitleLogo.h"

#include "../../GameObject/Title_Back/Title_Back.h"

#include "../../GameObject/UI/PressEnterUI/PressEnterUI.h"

#include "../GameScene/GameScene.h"


void TitleScene::Event()
{
	const std::shared_ptr<Title_Back> spBack		= m_wpBack.lock();
	const std::shared_ptr<Helicopter> spHelicopter = m_wpHelicopter.lock();

	if (spBack)
	{
		if (spBack->GetAlpha() >= spBack->GetAlphaMax())
		{
			SceneManager::Instance().SetNextScene(SceneManager::SceneType::Game);

			if(spHelicopter)
			{
				spHelicopter->SoundStop();
			}
		}
	}
}

void TitleScene::Init()
{
	//ShowCursor(false);

	std::atomic<bool> titleDone(false);
	std::thread titleTh(&TitleScene::TitleInit, this, std::ref(titleDone));

	std::atomic<bool> stageDone(false);
	std::thread stageTh(&TitleScene::StageInit, this, std::ref(stageDone));

	std::atomic<bool> charaDone(false);
	std::thread charaTh(&TitleScene::CharaInit, this, std::ref(stageDone));

	std::atomic<bool> backDone(false);
	std::thread backTh(&TitleScene::BackInit, this, std::ref(backDone));

	titleTh.join();
	stageTh.join();
	charaTh.join();
	backTh.join();
}

void TitleScene::TitleInit(std::atomic<bool>& done)
{
	// 空
	std::shared_ptr<Title_Sky> sky;
	sky = std::make_shared<Title_Sky>();
	sky->Init();
	m_objList.push_back(sky);

	// ヘリコプター
	std::shared_ptr<Helicopter> helicopter;
	helicopter = std::make_shared<Helicopter>();
	helicopter->Init();
	m_objList.push_back(helicopter);
	m_wpHelicopter = helicopter;

	// カメラ初期化
	std::shared_ptr<TitleCamera> camera;
	camera = std::make_shared<TitleCamera>();
	camera->Init();
	m_objList.push_back(camera);

	// タイトルロゴ
	std::shared_ptr<TitleLogo> logo;
	logo = std::make_shared<TitleLogo>();
	logo->Init();
	m_objList.push_back(logo);

	// エンターキーUI
	std::shared_ptr<PressEnterUI> enterUI;
	enterUI = std::make_shared<PressEnterUI>();
	enterUI->Init();
	m_objList.push_back(enterUI);

	// 黒画面
	std::shared_ptr<Title_Back> back;
	back = std::make_shared<Title_Back>();
	back->Init();
	m_objList.push_back(back);
	m_wpBack = back;

	done = true;
}

void TitleScene::StageInit(std::atomic<bool>& done)
{
	//=================================================================
	// ステージ関係初期化
	//=================================================================
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Ground/Ground_Bottom/Ground_Bottom.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Ground/Ground_UP/Ground_UP.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/RockWall/RockWall.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Building/Bulding_Main/Building_Main.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Building/Building_Roof/Building_Roof.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/ArmoredCar/ArmoredCar.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Grass/Grass.gltf");

	done = true;
}

void TitleScene::CharaInit(std::atomic<bool>& done)
{
	//=================================================================
	// キャラクター関係初期化
	//=================================================================

	// プレイヤー初期化
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Player/Player_LowerBody_3.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Player/Player_UpperBody_3.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Player/Player_Disarm_Holder.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Player/Player_Disarm_Pistol.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Player/Player_Ready_Pistol.gltf");
	
	// 敵初期化
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Enemy/Enemy_Main/Enemy_Main.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Enemy/Enemy_Gun/Enemy_Gun.gltf");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Enemy/Enemy_Gun_NoMagazine/Enemy_GunNoMagazine");
	KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Characters/Enemy/Enemy_Magazine/Enemy_Magazine");

	done = true;
}

void TitleScene::BackInit(std::atomic<bool>& done)
{
	KdAssets::Instance().m_textures.GetData("Asset/Textures/BlackWindow/BlackWindow.png");

	done = true;
}
