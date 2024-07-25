#include "Meet.h"

#include "../../Player/Player.h"
#include "../../../Scene/GameScene/GameScene.h"

void Meet::Init()
{
	m_spPoly = std::make_shared<KdSquarePolygon>();
	m_spPoly->SetMaterial("Asset/Textures/Meet.png");
	m_spPoly->SetScale(0.3f);

	m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);

	m_fType = FoodType::Meet;

	m_mLocalPos = Math::Matrix::CreateTranslation(0.f, 0.8f, 0.f);
}

void Meet::Update()
{
	std::shared_ptr<Player> player = m_owner->GetPlayer().lock();
	if (player)
	{
		m_targetMat = player->GetMatrix();
	}

	m_mWorld = m_mLocalPos * m_targetMat;
}
