#include "Player.h"

void Player::Init()
{
	m_spModel	= std::make_shared<KdModelData>();
	m_spModel->Load("Asset/Models/Characters/Player/Hamu.png");

	m_pos = {};

	CharaBase::Init();
}

void Player::Update()
{
}

void Player::PostUpdate()
{
	m_mTrans = Math::Matrix::CreateTranslation(m_pos);
	m_mWorld = m_mTrans;
}
