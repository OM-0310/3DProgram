#pragma once

#include "../UIBase.h"

class Reticle : public UIBase
{
public:

	Reticle			()				{}
	~Reticle		()	override	{}

	void Init		()	override;
	void DrawSprite	()	override;

	void SetActive	(bool _Active)	{ m_Active = _Active; }

private:

	bool m_Active;
};
