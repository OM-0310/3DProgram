#pragma once

class Water : public KdGameObject
{
public:

	Water			()				{}
	~Water			()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawLesson	()	override;

private:

	std::shared_ptr<KdModelWork>	m_spModel;

	Math::Matrix					m_mTrans;

	Math::Vector3					m_pos;

	Math::Vector2					m_UVOffset;
	Math::Matrix					m_mR;
};
