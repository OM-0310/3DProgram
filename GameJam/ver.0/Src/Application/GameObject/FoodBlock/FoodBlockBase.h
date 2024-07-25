#pragma once

class FoodBlockBase : public KdGameObject
{
public:

	FoodBlockBase			()				{}
	virtual ~FoodBlockBase	()	override	{}

	virtual void Init		()	override;
	virtual void Update		()	override;
	void DrawLit			()	override;

protected:

	std::shared_ptr<KdModelData>	m_spModel;

	Math::Vector3					m_pos;

	// 後で消す
	Math::Color						m_color;
};
