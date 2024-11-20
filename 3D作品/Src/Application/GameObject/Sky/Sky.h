#pragma once

class Sky : public KdGameObject
{
public:

	Sky				()				{}
	~Sky			()	override	{}

	void Init		()	override;
	void DrawUnLit	()	override;

private:

	std::shared_ptr<KdModelWork>	m_spModel	= nullptr;

	Math::Matrix					m_mTrans	= Math::Matrix::Identity;
	Math::Matrix					m_mScale	= Math::Matrix::Identity;

	Math::Vector3					m_pos		= Math::Vector3::Zero;
};
