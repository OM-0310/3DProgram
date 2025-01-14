#pragma once
#include "../../TerrainBase.h"

class Building_Main : public TerrainBase
{
public:

	Building_Main					()				{}
	~Building_Main					()	override	{}

	void Init						()	override;
	void DrawLit					()	override;
	//void GenerateDepthMapFromLight	()	override;

private:
};