#pragma once

#include "../../WeaponBase/WeaponBase.h"

class Pistol_Ready : public WeaponBase
{
public:

	Pistol_Ready					()				{}
	~Pistol_Ready					()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void ShotBullet					();

private:

	Math::Matrix	m_mLocalMuzzle	= Math::Matrix::Identity;

	Math::Vector3	m_readyPos		= Math::Vector3::Zero;

	bool			m_shotFlg		= false;
};
