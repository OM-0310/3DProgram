#include "TitleScene.h"
#include "../GameScene/GameScene.h"
#include "../SceneManager.h"

#include "../../GameObject/Terrains/Ground/Ground.h"
#include "../../GameObject/Terrains/Building/Building.h"


void TitleScene::Event()
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Game
		);
	}
}

void TitleScene::Init()
{
	ShowCursor(false);

	std::atomic<bool> stageDone(false);
	std::thread stageTh(&TitleScene::StageInit, this, std::ref(stageDone));

	std::atomic<bool> charaDone(false);
	std::thread charaTh(&TitleScene::CharaInit, this, std::ref(stageDone));

	stageTh.join();
	charaTh.join();
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
