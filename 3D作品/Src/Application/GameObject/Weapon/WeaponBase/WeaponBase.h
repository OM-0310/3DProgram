#pragma once

class Player;

class WeaponBase : public KdGameObject
{
public:

	WeaponBase						()				{}
	virtual ~WeaponBase				()	override	{ Release(); }

	virtual void Init				()	override	{}
	virtual void Update				()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void SetPlayer					(std::shared_ptr<Player> _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

protected:

	void Release					()				{ m_spModel = nullptr; }

	std::weak_ptr<Player>			m_wpPlayer;
	std::shared_ptr<KdModelWork>	m_spModel	= nullptr;

	Math::Matrix					m_mLocal	= Math::Matrix::Identity;
	Math::Matrix					m_mScale	= Math::Matrix::Identity;
	Math::Matrix					m_mRot		= Math::Matrix::Identity;

	Math::Vector3					m_localPos	= Math::Vector3::Zero;
	Math::Color						m_color		= kWhiteColor;

	float							m_alpha		= 1.0f;
	bool							m_holdFlg	= false;
};
