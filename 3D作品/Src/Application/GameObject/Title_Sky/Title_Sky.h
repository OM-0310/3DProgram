#pragma once

class Title_Sky : public KdGameObject
{
public:

	Title_Sky		()				{}
	~Title_Sky		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawUnLit	()	override;

	void SetAngleFlg(const bool& _angleFlg) { m_angleFlg = _angleFlg; }

private:

	std::shared_ptr<KdModelWork>	m_spModel	= nullptr;

	Math::Matrix					m_mScale	= Math::Matrix::Identity;
	Math::Matrix					m_mRot		= Math::Matrix::Identity;
	Math::Matrix					m_mTrans	= Math::Matrix::Identity;

	Math::Vector3					m_pos		= Math::Vector3::Zero;

	const float						m_angleSpd	= 0.1f;
	const float						m_angleMax	= 360.0f;
	float							m_angle		= 0.0f;

	bool							m_angleFlg	= false;
};
