#include "GameScene.h"
#include"../SceneManager.h"

#include "../../GameObject/Terrain/Terrain.h"
#include "../../GameObject/Character/Character.h"

#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/CCTVCamera/CCTVCamera.h"

void GameScene::Init()
{
	//===================================================================
	// ステージ初期化
	//===================================================================
	std::shared_ptr<Terrain> _terrain = std::make_shared<Terrain>();
	_terrain->Init();
	AddObject(_terrain);

	//===================================================================
	// キャラクター初期化
	//===================================================================
	std::shared_ptr<Character> _character = std::make_shared<Character>();
	_character->Init();
	AddObject(_character);

	//===================================================================
	// カメラ初期化
	//===================================================================
//	std::shared_ptr<FPSCamera>		_camera = std::make_shared<FPSCamera>();
//	std::shared_ptr<TPSCamera>		_camera = std::make_shared<TPSCamera>();
	std::shared_ptr<CCTVCamera>		_camera = std::make_shared<CCTVCamera>();
	_camera->Init();
	_camera->SetTarget(_character);
	_camera->RegistHitObject(_terrain);
	_character->SetCamera(_camera);
	AddObject(_camera);

	//===================================================================
	// 乱数実験場
	//===================================================================
	//int randRes[10] = {};

	//// 如何にrand()が偏るか
	//srand((unsigned)time(NULL));

	//for (int i = 0; i < 100000000; i++)
	//{
	//	int tmp = rand() % 10000;
	//	int idx = tmp / 1000;

	//	randRes[idx]++;
	//}
	////結果を出力
	//OutputDebugStringA("--------------------------------------\n");
	//for (int i = 0; i < 10; i++)
	//{
	//	std::stringstream ss;
	//	ss << "取得した値が" << i * 1000 << "～" << (i + 1) * 1000 <<
	//		"の件数 : \t" << randRes[i] << "\n";
	//	std::string str = ss.str();
	//	OutputDebugStringA(str.c_str());
	//}
	//OutputDebugStringA("--------------------------------------\n");

	// メルセンヌツイスタ!!!!!!!!!!!!!!!!!!!!!!!!!!

	//for (int i = 0; i < 100000000; i++)
	//{
	//	int tmp = KdGetInt(0, 9999);
	//	int idx = tmp / 1000;

	//	randRes[idx]++;
	//}
	////結果を出力
	//OutputDebugStringA("--------------------------------------\n");
	//for (int i = 0; i < 10; i++)
	//{
	//	std::stringstream ss;
	//	ss << "取得した値が" << i * 1000 << "～" << (i + 1) * 1000 <<
	//		"の件数 : \t" << randRes[i] << "\n";
	//	std::string str = ss.str();
	//	OutputDebugStringA(str.c_str());
	//}
	//OutputDebugStringA("--------------------------------------\n");

	// レッスンその1：CカードとRカードをそれぞれ50％の確率で起動時に表示せよ!
	// レッスンその2：CカードとRカードをそれぞれ99.5%(Cカード) 0.5%(Rカード)の確率で起動時に表示せよ!
	// レッスンその3：CカードとRカードとSRカードをそれぞれ34%,33%,33%の確率で起動時に起動せよ! 
	// レッスンその4：CカードとRカードとSRカードをそれぞれ50% 49.5% 0.5%の確率で起動時に表示せよ

	//結果を出力
	
	OutputDebugStringA("--------------------------------------\n");
	int _bunbo			= 1000;
	int _randNum[3]		= { 500,495,5 };
	int _ThusenNum		= 10000000;
	int _TousenNum[3]	= { 0,0,0 };
	for (int i = 0; i < _ThusenNum; i++)
	{
		int _rnd = KdGetInt(0, _bunbo - 1);
		for (int j = 0; j < std::size(_randNum); j++)
		{
			_rnd -= _randNum[j];
			if (_rnd < 0)
			{
				_TousenNum[j]++;
				break;
			}
		}
	}
	float prob = 0.f;
	for (int i = 0; i < 3; i++)
	{
		std::stringstream ss;
		prob = ((float)_TousenNum[i] / (float)_ThusenNum) * 100;
		round_n(prob, 3);
		switch (i)
		{
		case 0:
			ss << "Cカード当選回数 = " << _TousenNum[0] << " " <<
				"当選確率 = " << prob << "%" << "\n";
			break;
		case 1:
			ss << "Rカード当選回数 = " << _TousenNum[1] << " " <<
				"当選確率 = " << prob << "%" << "\n";
			break;
		case 2:
			ss << "SRカード当選回数 = " << _TousenNum[2] << " " <<
				"当選確率 = " << prob << "%" << "\n";
			break;
		default:
			break;
		}
		std::string str = ss.str();
		OutputDebugStringA(str.c_str());
	}
	OutputDebugStringA("--------------------------------------\n");

	//OutputDebugStringA("--------------------------------------\n");
	//int _bunbo = 1000;
	//int _rnd = KdGetInt(0, _bunbo - 1);

	//int _ThusenNum = 10000000;
	//int _TousenNum[2] = { 0,0 };
	//for (int i = 0; i < _ThusenNum; i++)
	//{
	//	int _rnd = KdGetInt(0, _bunbo - 1);
	//	if (_rnd <= 4)
	//	{
	//		_TousenNum[1]++;
	//	}
	//	else
	//	{
	//		_TousenNum[0]++;
	//	}
	//}
	//float prob = 0.f;
	//for (int i = 0; i < 2; i++)
	//{
	//	std::stringstream ss;
	//	prob = ((float)_TousenNum[i] / (float)_ThusenNum) * 100;
	//	round_n(prob, 3);
	//	switch (i)
	//	{
	//	case 0:
	//		ss << "Cカード当選回数 = " << _TousenNum[0] << " " <<
	//			"当選確率 = " << prob << "%" << "\n";
	//		break;
	//	case 1:
	//		ss << "Rカード当選回数 = " << _TousenNum[1] << " " <<
	//			"当選確率 = " << prob << "%" << "\n";
	//		break;
	//	default:
	//		break;
	//	}
	//	std::string str = ss.str();
	//	OutputDebugStringA(str.c_str());
	//}
	//OutputDebugStringA("--------------------------------------\n");

	//OutputDebugStringA("--------------------------------------\n");
	//int _ThusenNum = 10000000;
	//int _TousenNum[2] = { 0,0 };
	//for (int i = 0; i < _ThusenNum; i++)
	//{
	//	int _rnd = KdGetInt(0, 1);
	//	_TousenNum[_rnd]++;
	//}
	//float prob = 0.f;
	//for (int i = 0; i < 2; i++)
	//{
	//	std::stringstream ss;
	//	prob = ((float)_TousenNum[i] / (float)_ThusenNum) * 100;
	//	round_n(prob, 3);
	//	switch (i)
	//	{
	//	case 0:
	//		ss << "Cカード当選回数 = " << _TousenNum[0] << " " <<
	//			"当選確率 = " << prob << "%" << "\n";
	//		break;
	//	case 1:
	//		ss << "Rカード当選回数 = " << _TousenNum[1] << " " <<
	//			"当選確率 = " << prob << "%" << "\n";
	//		break;
	//	default:
	//		break;
	//	}
	//	std::string str = ss.str();
	//	OutputDebugStringA(str.c_str());
	//}
	//OutputDebugStringA("--------------------------------------\n");

	/*std::stringstream ss;
	int _rnd = KdGetInt(0, 1);
	if (_rnd == 0)
	{
		ss << "Cカード当選!" << "\n";
	}
	else
	{
		ss << "Rカード当選!" << "\n";
	}
	std::string str = ss.str();
	OutputDebugStringA(str.c_str());
	OutputDebugStringA("--------------------------------------\n");*/

}

void GameScene::Event()
{
}