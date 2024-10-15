#pragma once

class TerrainBase : public KdGameObject
{
public:

	TerrainBase								()				{}
	virtual ~TerrainBase					()	override	{ Release(); }
	
	virtual void Init						()	override;
	virtual void DrawLit					()	override;
	virtual void GenerateDepthMapFromLight	()	override;

	void SetModel(const std::shared_ptr<KdModelData>& _spModel) { m_spModel->SetModelData(_spModel); }

protected:

	void Release()				{ m_spModel = nullptr; }

protected:

	std::shared_ptr<KdModelWork>	m_spModel	= nullptr;

	Math::Matrix					m_mTrans	= Math::Matrix::Identity;
	Math::Matrix					m_mScale	= Math::Matrix::Identity;
	Math::Matrix					m_mRot		= Math::Matrix::Identity;

	Math::Vector3					m_pos		= Math::Vector3::Zero;
};
