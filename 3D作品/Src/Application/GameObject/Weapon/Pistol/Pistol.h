#pragma once
#include "../WeaponBase/WeaponBase.h"

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

	const Math::Vector3	m_unholdBasePoint	= { 0.f,1.4f,0.6f };
	const Math::Vector3 m_holdBasePoint		= { 0.f,1.4f,0.6f };
};
