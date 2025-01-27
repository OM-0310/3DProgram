#include "Helicopter.h"

void Helicopter::Init()
{
	if (!m_spModel)
	{
		KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Helicopter/Helicopter.gltf");
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData(KdAssets::Instance().m_modeldatas.GetData("Asset/Models/Helicopter/Helicopter.gltf"));
	}

	m_spAnimator = std::make_shared<KdAnimator>();
	m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation("Spin"));

	m_spSound = KdAudioManager::Instance().GetSoundInstance("Asset/Sounds/Title/Helicopter.wav");
	m_spSound->SetVolume(m_volume);
	m_spSound->Play();

	m_pos = { 0.0f,-2.5f,0.0f };

	m_angle = -90.0f;
}

void Helicopter::Update()
{
	m_mTrans	= Math::Matrix::CreateTranslation(m_pos);
	m_mRot = Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(m_angle));
	m_mWorld = m_mRot * m_mTrans;
}

void Helicopter::PostUpdate()
{
	m_spAnimator->AdvanceTime(m_spModel->WorkNodes());
}

void Helicopter::DrawLit()
{
	if (!m_spModel)return;
	KdShaderManager::Instance().m_StandardShader.DrawModel(*m_spModel, m_mWorld);
}
