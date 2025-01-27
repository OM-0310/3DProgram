#pragma once

class EnemyBullet : public KdGameObject
{
public:

	EnemyBullet	()				{}
	~EnemyBullet()	override	{}

	void Init	()	override;
	void Update	()	override;
	void DrawLit()	override;

	void Shot(const Math::Vector3 _pos, const Math::Vector3 _dir);

private:

	std::shared_ptr<KdModelData>	m_spModel;

	Math::Matrix					m_mRot			= Math::Matrix::Identity;
	Math::Matrix					m_mTrans		= Math::Matrix::Identity;
	Math::Matrix					m_mLocalTrail	= Math::Matrix::Identity;

	Math::Vector3					m_pos			= Math::Vector3::Zero;
	Math::Vector3					m_moveDir		= Math::Vector3::Zero;
	const Math::Vector3				m_localTrailPos = { 0.0f,0.0f,-2.0 };

	static const short				BULLETPOW		= 1;
	const short						m_lifeSpanMax	= 100;
	const short						m_lifeSpanMin	= 0;
	short							m_lifeSpan		= 0;
	const float						m_hitArea		= 0.4f;

	const float						m_moveSpeed		= 2.0f;

	KdTrailPolygon					m_tPoly;
};
