#include "SecretFileLocation.h"

#include "../../Item/SecretFile/SecretFile.h"

void SecretFileLocation::Init()
{
	UIBase::SetAsset("Asset/Textures/UI/MiniMap/CurrentLocation/CurrentLocation.png");

	m_rect			= { 0,0,static_cast<long>(m_spTex->GetWidth()),static_cast<long>(m_spTex->GetHeight()) };

	m_pos			= {};
	m_color			= { 0.0f,1.0f,0.0f,1.0f };

	for (int i = 0; i <= m_totalEachFlg; i++)
	{
		m_bitsEachFlg[i] = false;
	}
}

void SecretFileLocation::Update()
{
	const std::shared_ptr<SecretFile> spSecretFile = m_wpSecretFile.lock();
	Math::Vector3 parentPos;

	if (spSecretFile)
	{
		parentPos = spSecretFile->GetPos();
	}

	float x = parentPos.x * m_posMulti;
	float y = (parentPos.z * m_posMulti) + m_adjustZPos;

	m_pos = { x,y };

	m_color = { 1.0f,0.0f,0.0f,1.0f };
}

void SecretFileLocation::DrawSprite()
{
	if (m_bitsEachFlg[PermissionFlg])
	{
		if (!m_bitsEachFlg[ActiveFlg])return;
		KdShaderManager::Instance().m_spriteShader.DrawTex(m_spTex, static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rect.width, m_rect.height, &m_rect, &m_color);
	}
}
