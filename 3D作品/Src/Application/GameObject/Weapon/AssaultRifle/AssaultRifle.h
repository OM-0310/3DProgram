﻿#pragma once
#include "../WeaponBase/WeaponBase.h"

class AssaultRifle : public WeaponBase
{
public:

	AssaultRifle	()				{}
	~AssaultRifle	()	override	{}

	void Init		()	override;
	void Update		()	override;

private:

	const Math::Vector3	m_unholdBasePoint	= { -0.1f,1.4f,-0.5f };
	const Math::Vector3 m_holdBasePoint		= { -0.1f,1.4f,-0.5f };
};
