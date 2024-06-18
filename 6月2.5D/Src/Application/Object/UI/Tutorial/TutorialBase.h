#pragma once
#include "../UIBase.h"

class TutorialBase :public UIBase
{
public:

	TutorialBase(){}
	~TutorialBase()override { Release(); }

	void Init()override;
	void Update()override;
	void DrawLit()override;

private:

	void Release() { m_spPoly = nullptr; }

protected:

	std::shared_ptr<KdSquarePolygon> m_spPoly;

};
