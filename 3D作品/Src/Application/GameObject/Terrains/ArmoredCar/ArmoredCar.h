#pragma once

#include "../TerrainBase.h"

class ArmoredCar : public TerrainBase
{
public:

	ArmoredCar	()				{}
	~ArmoredCar	()	override	{ Release(); }

	void Init	()	override;
	void DrawLit()	override;

private:
};
