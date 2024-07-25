#pragma once

#include "../FoodBlockBase.h"

class MeetBlock : public FoodBlockBase
{
public:

	MeetBlock	()				{}
	~MeetBlock	()	override	{}

	void Init	()	override;
	void Update	()	override;

private:
};
