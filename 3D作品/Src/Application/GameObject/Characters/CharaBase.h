#pragma once

class CharaBase : public KdGameObject
{
public:

	CharaBase						()				{}
	~CharaBase						()	override	{ Release(); }

	void Init						()	override;
	void Update						()	override	{}
	void PostUpdate					()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

protected:

	void Release					()				{ m_spModel = nullptr; }

protected:

	std::shared_ptr<KdModelData>	m_spModel	= nullptr;

	Math::Matrix					m_mTrans	= Math::Matrix::Identity;
	Math::Matrix					m_mRot		= Math::Matrix::Identity;

	Math::Vector3					m_pos		= Math::Vector3::Zero;
	Math::Color						m_color		= kWhiteColor;

	float							m_gravity	= 0.f;
	float							m_alpha		= 1.f;
};
