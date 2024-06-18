#pragma once
#include "../TutorialBase.h"

class Tutorial_Reload :public TutorialBase
{
public:

	Tutorial_Reload(){}
	~Tutorial_Reload()override { Release(); }

	void Init()override;
	void Update()override;

private:

	void Release() { m_spPoly = nullptr; }

private:

};
