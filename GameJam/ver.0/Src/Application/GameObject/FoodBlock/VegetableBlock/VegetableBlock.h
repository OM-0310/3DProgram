#pragma once

#include "../FoodBlockBase.h"

class VegetableBlock : public FoodBlockBase
{
public:

	VegetableBlock	()				{}
	~VegetableBlock	()	override	{}

	void Init		()	override;
	void Update		()	override;

private:
};
