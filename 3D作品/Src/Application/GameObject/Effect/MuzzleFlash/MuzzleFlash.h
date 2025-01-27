#pragma once

class Player;

class MuzzleFlash : public KdGameObject
{
public:

	MuzzleFlash		()				{}
	~MuzzleFlash	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawBright	()	override;

	void SetPos(const Math::Vector3 _pos)
	{
		m_pos = _pos;
	}

private:

	std::shared_ptr<KdSquarePolygon>	m_spPoly;
	std::weak_ptr<Player>				m_wpPlayer;

	Math::Matrix						m_mTrans		= Math::Matrix::Identity;
	Math::Matrix						m_mRot			= Math::Matrix::Identity;

	Math::Vector3						m_dir			= Math::Vector3::Zero;
	Math::Vector3						m_pos			= Math::Vector3::Zero;

	const short							m_lifeSpanMax	= 1;
	const short							m_lifeSpanMin	= 0;
	short								m_lifeSpan		= 0;
};
