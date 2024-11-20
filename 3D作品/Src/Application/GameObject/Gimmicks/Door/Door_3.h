#pragma once

class Player;

class Door_3 : public KdGameObject
{
public:

	Door_3() {}
	~Door_3()	override {}

	void Init()	override;
	void Update()	override;
	void DrawLit()	override;
	void GenerateDepthMapFromLight()	override;

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

private:

	std::shared_ptr<KdModelWork>	m_spModel;
	std::weak_ptr<Player>			m_wpPlayer;

	Math::Color						m_debugColor	= kWhiteColor;

	Math::Matrix					m_mTrans		= Math::Matrix::Identity;

	Math::Vector3					m_pos			= Math::Vector3::Zero;
	Math::Vector3					m_eventPos		= Math::Vector3::Zero;

	const float						m_moveMax		= 2.5f;
	const float						m_moveMin		= -0.9f;
	const float						m_moveSpeed		= 0.1f;

	const float						m_openArea		= 1.4f;

	bool							m_openAbleFlg	= false;
	bool							m_openFlg		= false;
};
