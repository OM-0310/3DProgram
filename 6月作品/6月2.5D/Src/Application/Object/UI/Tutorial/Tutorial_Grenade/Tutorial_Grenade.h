#pragma once
#include "../TutorialBase.h"

class Tutorial_Grenade :public TutorialBase
{
public:

	Tutorial_Grenade(){}
	~Tutorial_Grenade()override { Release(); }

	void Init()override;
	void Update()override;

private:

	void Release() { m_spPoly = nullptr; }

private:
};
