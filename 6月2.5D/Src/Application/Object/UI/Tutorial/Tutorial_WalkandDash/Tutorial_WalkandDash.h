#pragma once
#include "../TutorialBase.h"

class Tutoria_WalkandDash :public TutorialBase
{
public:

	Tutoria_WalkandDash(){}
	~Tutoria_WalkandDash()override{}

	void Init()override;
	void Update()override;

private:

	void Release() { m_spPoly = nullptr; }

private:
};
