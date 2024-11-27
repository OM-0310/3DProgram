#pragma once

#include "../../TerrainBase.h"

class Ground_UP : public TerrainBase
{
public:

	Ground_UP	()				{}
	~Ground_UP	()	override	{}

	void Init	()	override;
	void DrawLit()	override;

private:
};
