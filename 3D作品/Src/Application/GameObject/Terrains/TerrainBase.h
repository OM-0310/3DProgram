#pragma once

class TerrainBase : public KdGameObject
{
public:

	TerrainBase								()				{}
	virtual ~TerrainBase					()	override	{ Release(); }
	
	virtual void Init						()	override;
	virtual void DrawLit					()	override;
	virtual void GenerateDepthMapFromLight	()	override	{}

	void SetAsset(std::string _name);

protected:

	void Release()				{ m_spModel = nullptr; }

protected:

	std::shared_ptr<KdModelWork>	m_spModel	= nullptr;					// モデル情報

	Math::Matrix					m_mTrans	= Math::Matrix::Identity;	// 座標行列
	Math::Matrix					m_mScale	= Math::Matrix::Identity;	// 拡縮行列
	Math::Matrix					m_mRot		= Math::Matrix::Identity;	// 回転行列

	Math::Vector3					m_pos		= Math::Vector3::Zero;		// 座標
};
