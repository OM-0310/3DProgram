#pragma once

class Player;

class ExclamationMarkUI : public KdGameObject
{
public:

	ExclamationMarkUI	()				{}
	~ExclamationMarkUI	()	override	{}

	void Init			()	override;
	void Update			()	override;
	void DrawUnLit		()	override;

	void SetPos(const Math::Vector3 _pos)
	{
		m_pos = _pos;
	}

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

private:

	std::shared_ptr<KdSquarePolygon>	m_spPoly;
	std::weak_ptr<Player>				m_wpPlayer;

	Math::Matrix						m_mTrans		= Math::Matrix::Identity;
	Math::Matrix						m_mRot			= Math::Matrix::Identity;

	Math::Vector3						m_dir			= Math::Vector3::Zero;
	Math::Vector3						m_pos			= Math::Vector3::Zero;
	const Math::Vector3					m_adjustPos		= { 0.0f,2.5f,0.0f };

	const short							m_lifeSpanMax	= 60;
	const short							m_lifeSpanMin	= 0;
	short								m_lifeSpan		= 0;
};
