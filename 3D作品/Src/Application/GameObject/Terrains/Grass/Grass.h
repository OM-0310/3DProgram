#pragma once

#include "../TerrainBase.h"

class Grass : public TerrainBase
{
public:

	Grass							()				{}
	~Grass							()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void SetPos(const Math::Vector3 _pos) { m_pos = _pos; }

private:
};
