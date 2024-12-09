#pragma once

#include "../TerrainBase.h"

class RockWall : public TerrainBase
{
public:

	RockWall						()				{}
	~RockWall						()	override	{}

	void Init						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

private:
};
