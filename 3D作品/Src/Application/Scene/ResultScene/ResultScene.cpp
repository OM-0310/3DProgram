#include "ResultScene.h"

#include "../../GameObject/Result_Back/Result_Back.h" 

#include "../../GameObject/Camera/TitleCamera/TitleCamera.h"

#include "../../GameObject/Title_Sky/Title_Sky.h"
#include "../../GameObject/Helicopter/Helicopter.h"
#include "../../GameObject/ResultLogo/ResultLogo.h"

#include "../../Scene/SceneManager.h"

void ResultScene::Event()
{
	const std::shared_ptr<Result_Back> spBack = m_wpBack.lock();

	if (spBack)
	{
		if (spBack->GetAlpha() >= spBack->GetAlphaMax())
		{
			SceneManager::Instance().SetNextScene
			(
				SceneManager::SceneType::Title
			);
		}
	}
}

void ResultScene::Init()
{
	std::shared_ptr<TitleCamera> camera;
	camera = std::make_shared<TitleCamera>();
	camera->Init();
	m_objList.push_back(camera);

	std::shared_ptr<Title_Sky> sky;
	sky = std::make_shared<Title_Sky>();
	sky->Init();
	sky->SetAngleFlg(true);
	m_objList.push_back(sky);

	std::shared_ptr<Helicopter> heli;
	heli = std::make_shared<Helicopter>();
	heli->Init();
	heli->SetAngle(90);
	m_objList.push_back(heli);

	std::shared_ptr<ResultLogo> logo;
	logo = std::make_shared<ResultLogo>();
	logo->Init();
	m_objList.push_back(logo);

	std::shared_ptr<Result_Back> back;
	back = std::make_shared<Result_Back>();
	back->Init();
	m_objList.push_back(back);
	m_wpBack = back;
}
