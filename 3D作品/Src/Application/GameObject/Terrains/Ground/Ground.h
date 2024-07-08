#pragma once
#include "../TerrainBase.h"

class Ground : public TerrainBase
{
public:

	Ground		()				{}
	~Ground		()	override	{ Release(); }

	void Init	()	override;

private:
};
