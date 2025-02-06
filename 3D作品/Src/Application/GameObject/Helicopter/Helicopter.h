#pragma once

class Helicopter : public KdGameObject
{
public:

	Helicopter		()				{}
	~Helicopter		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void PostUpdate	()	override;
	void DrawLit	()	override;

	void SoundStop	() { m_spSound->Stop(); }

	void SetAngle(const float _angle) { m_angle = _angle; }

private:

	std::shared_ptr<KdModelWork>		m_spModel;
	std::shared_ptr<KdAnimator>			m_spAnimator;

	Math::Matrix						m_mRot		= Math::Matrix::Identity;
	Math::Matrix						m_mTrans	= Math::Matrix::Identity;

	Math::Vector3						m_pos		= Math::Vector3::Zero;

	float								m_angle		= 0.0f;

	static constexpr float				m_volume	= 0.1f;
	std::shared_ptr<KdSoundInstance>	m_spSound;
};
