#pragma once

class TerrainBase : public KdGameObject
{
public:

	TerrainBase	()				{}
	~TerrainBase()	override	{ Release(); }
	
	void Init	()	override;
	void DrawLit()	override;

protected:

	void Release()				{ m_spModel = nullptr; }

protected:

	std::shared_ptr<KdModelData>	m_spModel	= nullptr;


	Math::Matrix					m_mTrans	= Math::Matrix::Identity;
	Math::Matrix					m_mRot		= Math::Matrix::Identity;

	Math::Vector3					m_pos		= Math::Vector3::Zero;
};
