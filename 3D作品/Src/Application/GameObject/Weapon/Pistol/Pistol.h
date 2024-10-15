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

	Math::Matrix m_mHold;
	Math::Matrix m_mUnHold;

	Math::Matrix m_mMuzzle;
};
