#pragma once
#include "../../TerrainBase.h"

class Building_Roof : public TerrainBase
{
public:

	Building_Roof	()			{}
	~Building_Roof	()override	{}

	void Init()		override;
	void DrawLit()	override;

private:

};
