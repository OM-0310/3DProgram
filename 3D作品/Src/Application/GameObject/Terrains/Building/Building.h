#pragma once
#include "../TerrainBase.h"

class Building : public TerrainBase
{
public:

	Building						()			{}
	~Building						()	override{}

	void Init						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

private:
};
