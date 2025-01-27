#include "GoalUI.h"

#include "../../Camera/TPSCamera/TPSCamera.h"

#include "../../Goal/Goal.h"

void GoalUI::Init()
{
	if (!m_spPoly)
	{
		KdAssets::Instance().m_textures.GetData("Asset/Textures/UI/MiniMap/CurrentLocation/CurrentLocation.png");
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial(KdAssets::Instance().m_textures.GetData("Asset/Textures/UI/MiniMap/CurrentLocation/CurrentLocation.png"));
		m_spPoly->SetScale(0.75f);
	}
}

void GoalUI::Update()
{
	const std::shared_ptr<TPSCamera>	spCamera= m_wpCamera.lock();
	const std::shared_ptr<Goal>			spGoal	= m_wpGoal.lock();
	Math::Matrix cameraMat;

	if (spCamera)
	{
		cameraMat = spCamera->GetMatrix();
	}

	// プレイヤー(カメラ)と同じ軸を代入して方向を合わせる
	m_mWorld.Backward(cameraMat.Backward());
	m_mWorld.Up(cameraMat.Up());
	m_mWorld.Right(cameraMat.Right());

	if (spGoal)
	{
		m_mWorld.Translation(spGoal->GetPos() + Math::Vector3{ 0.0f,2.0f,0.0f });
	}
}

void GoalUI::DrawUnLit()
{
	const std::shared_ptr<Goal>			spGoal = m_wpGoal.lock();

	if (spGoal)
	{
		if (!spGoal->GetGoalAbleFlg())return;
		KdShaderManager::Instance().m_StandardShader.DrawPolygon(*m_spPoly, m_mWorld);
	}
}
