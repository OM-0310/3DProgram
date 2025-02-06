#include "LockedDoor.h"

#include "../../Characters/Player/Player.h"

#include "../../UI/CollectUIBack/CollectUIBack.h"
#include "../../UI/NeedCardKeyUI/NeedCradKeyUI.h"
#include "../../UI/SubMissionUI/SubMissionUI.h"

#include "../../../Scene/SceneManager.h"

void LockedDoor::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Gimmicks/Door/Door.gltf");
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Gimmicks/Door/Door.gltf"));
	}
	
	m_spOpenSound = KdAudioManager::Instance().GetSoundInstance3D("Asset/Sounds/Game/OpenDoor.wav");
	m_spOpenSound->SetVolume(m_openVol);

	m_debugColor	= kRedColor;

	m_pos			= { 4.5f,-0.9f,-18.f };
	m_eventPos		= { 4.f,0.f,-18.f };

	for (int i = 0; i <= m_totalEachFlg; i++)
	{
		m_bitsEachFlg[i] = false;
	}

	m_objectType = KdGameObject::ObjectType::TypeObstacles;

	m_pCollider		= std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("DoorCollsion", m_spModel, KdCollider::TypeGround);

	m_pDebugWire	= std::make_unique<KdDebugWireFrame>();
}

void LockedDoor::Update()
{
	const std::shared_ptr<Player>		spPlayer		= m_wpPlayer.lock();
	const std::shared_ptr<SubMissionUI>	spSubMissionUI	= m_wpSubMissionUI.lock();

	//// //// //// //// //// //// //// //// //// //// //// //// //// //// ////
	// 扉の開閉処理(円判定を用いて実装)										//
	//// //// //// //// //// //// //// //// //// //// //// //// //// //// ////

	KdCollider::SphereInfo sphere;
	sphere.m_sphere.Center = m_eventPos;
	sphere.m_sphere.Radius = m_openArea;
	sphere.m_type = KdCollider::TypeEvent;

	// デバッグ用
	//m_pDebugWire->AddDebugSphere(sphere.m_sphere.Center, sphere.m_sphere.Radius, m_debugColor);

	bool isHit = false;
	for (auto& obj : SceneManager::Instance().GetObjList())
	{
		// オブジェクトタイプがプレイヤーだったら
		if (obj->GetObjectType() == KdGameObject::ObjectType::TypePlayer)
		{
			// ヒットフラグをtrueにする
			isHit = obj->Intersects(sphere, nullptr);

			// ヒットフラグがtrueだったら
			if (isHit)
			{
				// プレイヤーの情報があれば
				if (spPlayer)
				{
					// プレイヤーがカードキーを所持している状態であれば
					if (spPlayer->GetItemCollType() == Player::ItemCollectType::CardKeyCollect)
					{
						// 開錠可能フラグをtrueしてループを抜ける
						m_bitsEachFlg[OpenAbleFlg] = true;
						m_debugColor = kGreenColor;
					}
					// そうでなければ
					else
					{	
						// ロックフラグがtrueであれば
						if (m_bitsEachFlg[LockFlg])
						{
							// 開錠可能フラグをtrueしてループを抜ける
							m_bitsEachFlg[OpenAbleFlg] = true;
							m_debugColor = kGreenColor;

						}
						// falseであれば
						else
						{
							if (!m_bitsEachFlg[UIFlg])
							{
								m_bitsEachFlg[UIFlg] = true;

								GenerateNeedCardKeyUI();

								if (spSubMissionUI)
								{
									spSubMissionUI->Permission();
								}
							}

							// 開錠可能フラグをfalseしてループを抜ける
							m_bitsEachFlg[OpenAbleFlg] = false;

							m_debugColor = kBlueColor;
						}
					}
				}
				break;
			}
			else
			{
				// 開錠可能フラグをfalseにしてループを抜ける
				m_bitsEachFlg[OpenAbleFlg] = false;
				m_debugColor	= kRedColor;
			}
		}
	}

	// ロックフラグがtrueであれば
	if (m_bitsEachFlg[LockFlg])
	{
		// 開錠可能フラグがtrueであれば
		if (m_bitsEachFlg[OpenAbleFlg])
		{
			// 開錠フラグをtrueにする
			m_bitsEachFlg[OpenFlg] = true;

			if (m_spOpenSound->IsStopped())
			{
				m_spOpenSound->Play();
			}
		}
	}

	// 開錠フラグがtrueであれば
	if (m_bitsEachFlg[OpenFlg])
	{
		// 扉のY座標の値をプラスする
		m_pos.y += m_moveSpeed;
		// 扉のY座標の値が2.5以上であれば
		if (m_pos.y >= m_moveMax)
		{
			// 扉のY座標の値を2.5に留める
			m_pos.y = m_moveMax;

			// 開錠可能フラグがtrueであれば
			if (m_bitsEachFlg[OpenAbleFlg])
			{
				// 開錠フラグをfalseにする
				m_bitsEachFlg[OpenFlg] = false;

				m_spOpenSound->Play();
			}
		}
	}
	// falseであれば
	else
	{
		// 扉のY座標の値をマイナスする
		m_pos.y -= m_moveSpeed;
		// 扉のY座標の値が-0,9以下であれば
		if (m_pos.y <= m_moveMin)
		{
			// 扉のY座標の値を-0.9に留める
			m_pos.y = m_moveMin;
		}
	}

	// 行列の作成
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;

	m_spOpenSound->SetPos(GetPos());
}

void LockedDoor::DrawLit()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void LockedDoor::GenerateDepthMapFromLight()
{
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}

void LockedDoor::Open()
{
	m_bitsEachFlg[LockFlg] = true;
}

void LockedDoor::GenerateNeedCardKeyUI()
{
	std::shared_ptr<CollectUIBack> back;
	back = std::make_shared<CollectUIBack>();
	back->Init();
	SceneManager::Instance().AddObject(back);

	std::shared_ptr<NeedCardKeyUI> needUI;
	needUI = std::make_shared<NeedCardKeyUI>();
	needUI->Init();
	SceneManager::Instance().AddObject(needUI);
}
