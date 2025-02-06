#pragma once

#include "../../../TerrainBase.h"

class Bed : public TerrainBase
{
public:

	Bed			()				{}
	~Bed		()	override	{}

	void Init	()	override;
	void Update	()	override;

	void SetPos		(const Math::Vector3& _pos) { m_pos = _pos;		}
	void SetAngle	(const float _angle)		{ m_angle = _angle; }

private:

	float m_angle = 0.0f;;
};
