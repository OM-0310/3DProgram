#pragma once
#include "../../WeaponBase/WeaponBase.h"

class Pistol_Disarm : public WeaponBase
{
public:

	Pistol_Disarm					()				{}
	~Pistol_Disarm					()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

private:

	Math::Matrix	m_mlocalRot;

	Math::Vector3	m_disarmPos;
};
