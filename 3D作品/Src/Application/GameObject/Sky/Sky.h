#pragma once

class Sky : public KdGameObject
{
public:

	Sky				()				{}
	~Sky			()	override	{}

	void Init		()	override;
	void DrawUnLit	()	override;

private:

	std::shared_ptr<KdModelWork>	m_spModel;

	Math::Matrix					m_mTrans;
	Math::Matrix					m_mScale;

	Math::Vector3					m_pos;
};
