#pragma once

#include "../ItemBase.h"

class Player;

class SecretFile : public ItemBase
{
public:

	SecretFile						()				{}
	~SecretFile						()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void DrawBright					()	override;
	void GenerateDepthMapFromLight	()	override;

private:
};
