#pragma once

class FoodBase : public KdGameObject
{
public:

	FoodBase			()				{}
	virtual ~FoodBase	()	override	{}

	virtual void Init	()	override;
	virtual void Update	()	override;
	void DrawLit		()	override;

	void Delete			()				{ m_isExpired = true; }

protected:

	Math::Matrix						m_targetMat;

	std::shared_ptr<KdSquarePolygon>	m_spPoly;

	Math::Matrix						m_mLocalPos;
};
