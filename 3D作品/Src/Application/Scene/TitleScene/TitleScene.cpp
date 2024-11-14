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

	stageTh.join();
}

void TitleScene::StageInit(std::atomic<bool>& done)
{
	//=================================================================
	// ステージ関係初期化・・・ここから
	//=================================================================

	// 地形　地面初期化
	//m_spGroundModel		= KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Ground/Ground.gltf");
	
	//std::shared_ptr<KdModelData> m =  KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Ground/Ground.gltf");

	//std::shared_ptr<KdModelWork> w;
	//w->SetModelData(m);

	// 建物
	//std::shared_ptr<KdModelData> build = KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Terrains/Ground.Ground.gltf");

	//=================================================================
	// ステージ関係初期化・・・ここまで
	//=================================================================

	done = true;
}
