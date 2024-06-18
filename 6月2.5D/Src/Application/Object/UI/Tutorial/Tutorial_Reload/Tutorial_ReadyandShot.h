#pragma once
#include "../TutorialBase.h"

class Tutorial_ReadyandShot :public TutorialBase
{
public:

	Tutorial_ReadyandShot(){}
	~Tutorial_ReadyandShot()override { Release(); }

	void Init()override;
	void Update()override;

private:

	void Release() { m_spPoly = nullptr; }

private:
};
