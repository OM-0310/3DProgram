#pragma once
#include "../CharaBase.h"

class Player : public CharaBase
{
public:

	Player			()				{}
	~Player			()	override	{ Release(); }

	void Init		()	override;
	void Update		()	override;
	void PostUpdate	()	override;

private:
};
