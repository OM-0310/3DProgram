#pragma once

#include "../FoodBase.h"

class GameScene;

class Meet : public FoodBase
{
public:

	Meet		()				{}
	~Meet		()	override	{}

	void Init	()	override;
	void Update	()	override;

	void SetOwner(GameScene* _owner) { m_owner = _owner; }

private:

	GameScene* m_owner;
};
