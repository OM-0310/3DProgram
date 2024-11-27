#pragma once

#include "../../TerrainBase.h"

class Ground_Bottom : public TerrainBase
{
public:

	Ground_Bottom	()				{}
	~Ground_Bottom	()	override	{}

	void Init		()	override;
	void DrawLit	()	override;

private:
};
