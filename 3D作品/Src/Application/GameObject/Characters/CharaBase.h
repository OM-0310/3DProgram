#pragma once

class CharaBase : public KdGameObject
{
public:

	CharaBase						()				{}
	virtual ~CharaBase				()	override	{ Release(); }

	virtual void Init				()	override;
	virtual void Update				()	override	{}
	virtual void PostUpdate			()	override;
	virtual void DrawUnLit			()	override	{}
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void SetAsset(std::string _name);

	const std::shared_ptr<KdModelWork>& GetModel() const
	{
		return m_spModel;
	}

protected:

	void Release					()				{ m_spModel = nullptr; }
	void UpdateRotate				(const Math::Vector3& srcMoveVec);

protected:

	std::shared_ptr<KdModelWork>		m_spModel		= nullptr;					// モデル情報

	Math::Matrix						m_mTrans		= Math::Matrix::Identity;	// 座標行列
	Math::Matrix						m_mRot			= Math::Matrix::Identity;	// 回転行列

	Math::Vector3						m_pos			= Math::Vector3::Zero;		// 座標
	Math::Vector3						m_moveDir		= Math::Vector3::Zero;		// 移動方向
	float								m_moveSpeed		= 0.f;						// 移動速度

	Math::Vector3						m_angle			= Math::Vector3::Zero;		// 角度

	Math::Color							m_color			= kWhiteColor;				// 色情報
	float								m_alpha			= 1.f;						// アルファ値
	const float							m_alphaMax		= 1.f;						// アルファ値最大値
	const float							m_alphaMin		= 0.f;						// アルファ値最小値

	float								m_gravity		= 0.f;						// 重力
};
