#pragma once

class CharaBase : public KdGameObject
{
public:

	CharaBase						()				{}
	virtual ~CharaBase				()	override	{ Release(); }

	virtual void Init				()	override;
	virtual void Update				()	override	{}
	virtual void PostUpdate			()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

protected:

	void Release					()				{ m_spModel = nullptr; }
	void UpdateRotate				(const Math::Vector3& srcMoveVec);

protected:

	std::shared_ptr<KdModelWork>	m_spModel	= nullptr;					// モデル情報

	Math::Matrix					m_mTrans	= Math::Matrix::Identity;	// 座標行列
	Math::Matrix					m_mRot		= Math::Matrix::Identity;	// 回転行列

	Math::Vector3					m_pos		= Math::Vector3::Zero;		// 座標
	Math::Vector3					m_angle		= Math::Vector3::Zero;		// 角度
	Math::Color						m_color		= kWhiteColor;				// 色情報

	float							m_gravity	= 0.f;						// 重力
	float							m_alpha		= 1.f;						// アルファ値
};
