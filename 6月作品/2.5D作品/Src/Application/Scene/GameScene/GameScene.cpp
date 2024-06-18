#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Object/Ground/Ground.h"

void GameScene::Event()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		SceneManager::Instance().SetNextScene
		(
			SceneManager::SceneType::Title
		);
	}

	Math::Matrix transMat;
	Math::Vector3 cameraPos = { 3.0f,1.5f,-5.0f };
	transMat = Math::Matrix::CreateTranslation(cameraPos);
	m_camera->SetCameraMatrix(transMat);
}

void GameScene::Init()
{
	// ==カメラ=========================================
	m_camera = std::make_unique<KdCamera>();

	// ==地面===========================================
	std::shared_ptr<Ground> ground;
	ground = std::make_shared<Ground>();
	m_objList.push_back(ground);
}
