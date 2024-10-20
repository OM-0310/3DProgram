#pragma once

#include "../../WeaponBase/WeaponBase.h"

class Pistol_Ready : public WeaponBase
{
public:

	Pistol_Ready					()				{}
	~Pistol_Ready					()	override	{}

	void Init						()	override;
	void Update						()	override;

	void ShotBullet					();

private:

	Math::Matrix	m_mLocalMuzzle;

	Math::Vector3	m_readyPos;

	bool			m_shotFlg;
};
