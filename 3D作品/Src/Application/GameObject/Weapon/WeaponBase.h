#pragma once

class CharaBase;

class WeaponBase : public KdGameObject
{
public:

	enum class WeaponType
	{
		Pistol,
	};

	WeaponBase						()				{}
	virtual ~WeaponBase				()	override	{ Release(); }

	virtual void Init				()	override	{}
	virtual void Update				()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	virtual void Hold				() = 0;
	virtual void UnHold				() = 0;

	void SetChara					(std::shared_ptr<CharaBase> _spChara)
	{
		m_wpChara = _spChara;
	}

	const WeaponType GetWeaponType	() const		{ return m_weponType; }

protected:

	void Release					()				{ m_spModel = nullptr; }

	std::weak_ptr<CharaBase>		m_wpChara;
	std::shared_ptr<KdModelWork>	m_spModel	= nullptr;

	Math::Matrix					m_mLocal	= Math::Matrix::Identity;
	Math::Matrix					m_mScale	= Math::Matrix::Identity;
	Math::Matrix					m_mRot		= Math::Matrix::Identity;

	Math::Vector3					m_localPos	= Math::Vector3::Zero;

	bool							m_holdFlg	= false;

	WeaponType						m_weponType;
};
