#pragma once
#include "../WeaponBase.h"

class Pistol : public WeaponBase
{
public:

	Pistol		()				{}
	~Pistol		()	override	{}

	void Init	()	override;
	void Update	()	override;

	void Hold	()	override	{ m_holdFlg = true; }
	void UnHold	()	override	{ m_holdFlg = false; }

private:

	const Math::Vector3	m_unholdBasePoint	= { -0.3f,1.5f,-1.5f };
	const Math::Vector3 m_holdBasePoint		= { 0.f,1.67f,-1.1f };
};
