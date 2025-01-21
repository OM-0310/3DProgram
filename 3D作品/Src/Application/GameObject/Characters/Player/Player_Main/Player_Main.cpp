#include "Player_Main.h"

#include "../Player.h"

#include "../../../../main.h"

void Player_Main::Init()
{
	CharaBase::SetAsset("Asset/Models/Characters/Player/Player_Main/Player_Main.gltf");
	m_spAnimator = std::make_shared<KdAnimator>();

	ChangeAnimation("Idle");

	m_pos = { 0.f,-0.9f,-50.0f };

	m_HP = m_maxHP;

	m_objectType = KdGameObject::ObjectType::TypePlayer;

	m_pCollider = std::make_unique<KdCollider>();
	m_pCollider->RegisterCollisionShape("Player_MainCollsion", m_spModel, KdCollider::TypeEvent | KdCollider::TypeBump | KdCollider::TypeDamage);
}

void Player_Main::Update()
{
	const std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer)
	{
		m_mWorld = spPlayer->GetMatrix();
	}

	Application::Instance().m_log.Clear();
	Application::Instance().m_log.AddLog("m_hp=%d", m_HP);
}

void Player_Main::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
	m_spModel->CalcNodeMatrices();
}