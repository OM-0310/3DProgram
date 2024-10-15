#include "Pistol.h"

void Pistol::Init()
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
		m_spModel->SetModelData("Asset/Models/Weapon/Pistol/Pistol_1.gltf");
	}

	if (m_spModel)
	{
		const KdModelWork::Node* pNode = m_spModel->FindNode("MuzzlePoint");

		if (pNode)
		{
			m_mMuzzle = pNode->m_worldTransform * m_mLocal;
		}
		else if (!pNode)
		{
			assert(0 && "MuzzlePoint 指定したノードが見つかりません");
		}
	}
}

void Pistol::Update()
{

	WeaponBase::Update();
}
