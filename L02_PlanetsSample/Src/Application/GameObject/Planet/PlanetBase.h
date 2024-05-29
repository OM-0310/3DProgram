#pragma once

class PlanetBase : public KdGameObject
{
public:
	PlanetBase();
	~PlanetBase()						override;

	void Init()							override;
	void Update()						override;
	void DrawLit()						override;
	void GenerateDepthMapFromLight()	override;

	void SetParentPlanet(const std::shared_ptr<PlanetBase> parent)
	{
		m_wpParent = parent;
	}

private:

	void Release();

protected:
	// KdModelData…アニメーションをしないときや内部データをいじる時に利用する
	// KdModelWork…アニメーションしたいときや内部データをいじる場合のみ利用する

	std::shared_ptr<KdModelWork>		m_spModel	= nullptr;
	std::weak_ptr<PlanetBase>			m_wpParent;

	Math::Matrix						m_mTrans	= Math::Matrix::Identity;
	Math::Matrix						m_mRotation = Math::Matrix::Identity;

};