#pragma once

#include "../ItemBase.h"

class Player;

class CardKey : public ItemBase
{
public:

	CardKey							()				{}
	~CardKey						()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void DrawBright					()	override;
	void GenerateDepthMapFromLight	()	override;

private:
};
