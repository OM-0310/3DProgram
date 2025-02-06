#pragma once

class Player;

class Door_2 : public KdGameObject
{
public:

	Door_2							() {}
	~Door_2							()	override {}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

private:

	enum
	{
		OpenAbleFlg,
		OpenFlg
	};

private:

	std::shared_ptr<KdModelWork>	m_spModel;

	Math::Color						m_debugColor	= kWhiteColor;

	Math::Matrix					m_mTrans		= Math::Matrix::Identity;

	Math::Vector3					m_pos			= Math::Vector3::Zero;
	Math::Vector3					m_eventPos		= Math::Vector3::Zero;

	static constexpr float			m_moveMax		= 2.5f;
	static constexpr float			m_moveMin		= -0.9f;
	static constexpr float			m_moveSpeed		= 0.1f;

	static constexpr float			m_openArea		= 1.4f;

	static constexpr short			m_totalEachFlg = 2;
	std::bitset<m_totalEachFlg>		m_bitsEachFlg;

	static constexpr float				m_openVol = 1.0f;
	std::shared_ptr<KdSoundInstance3D>	m_spOpenSound;
};
