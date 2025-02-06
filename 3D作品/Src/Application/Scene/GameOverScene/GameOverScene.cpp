#include "GameOverScene.h"

#include "../../GameObject/UI/GameOver/MissionFailedBack/MissionFailedBack.h"
#include "../../GameObject/UI/GameOver/SelectTitle/SelectTitle.h"
#include "../../GameObject/UI/GameOver/SelectRetry/SelectRetry.h"
#include "../../GameObject/UI/GameOver/GameOver_Back/GameOver_Back.h"

#include "../SceneManager.h"

void GameOverScene::Event()
{
	const std::shared_ptr<GameOver_Back>	spBack	= m_wpBack.lock();
	const std::shared_ptr<SelectTitle>		spTitle = m_wpTitle.lock();
	const std::shared_ptr<SelectRetry>		spRetry = m_wpRetry.lock();

	if (spBack && spTitle && spRetry)
	{
		if (spBack->GetAlpha() >= spBack->GetAlphaMax() && spTitle->GetChoiceFlg())
		{
			SceneManager::Instance().SetNextScene
			(
				SceneManager::SceneType::Title
			);
		}
		else if(spBack->GetAlpha() >= spBack->GetAlphaMax() && spRetry->GetChoiceFlg())
		{
			SceneManager::Instance().SetNextScene
			(
				SceneManager::SceneType::Game
			);
		}
	}
}

void GameOverScene::Init()
{
	std::shared_ptr<MissionFailedUIBack> failBack;
	failBack = std::make_shared<MissionFailedUIBack>();
	failBack->Init();
	m_objList.push_back(failBack);

	std::shared_ptr<SelectTitle> title;
	title = std::make_shared<SelectTitle>();
	title->Init();
	m_objList.push_back(title);
	m_wpTitle = title;

	std::shared_ptr<SelectRetry> retry;
	retry = std::make_shared<SelectRetry>();
	retry->Init();
	m_objList.push_back(retry);
	m_wpRetry = retry;

	std::shared_ptr<GameOver_Back> back;
	back = std::make_shared<GameOver_Back>();
	back->Init();
	m_objList.push_back(back);
	m_wpBack = back;

	back->SetSelectTitle(title);
	back->SetSelectRetry(retry);
}
