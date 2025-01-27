#pragma once

#include "../../TerrainBase.h"

class Light : public TerrainBase
{
public:

	Light			()				{}
	~Light			()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawLit	()	override;
	void DrawBright	()	override;

	void SetPos(const Math::Vector3& _pos)
	{
		m_pos = _pos;
	}

private:
};
