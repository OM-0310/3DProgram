#pragma once

#include "../FoodBase.h"

class GameScene;

class Vegetable : public FoodBase
{
public:

	Vegetable		()					{}
	~Vegetable		()	override		{}

	void Init		()	override;
	void Update		()	override;

	void SetOwner	(GameScene* _owner) { m_owner = _owner; }

private:

	GameScene* m_owner;
};
