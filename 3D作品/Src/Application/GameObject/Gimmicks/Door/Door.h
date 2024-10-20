#pragma once

class Player;

class Door : public KdGameObject
{
public:

	Door							()				{}
	~Door							()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void Open();

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const bool& GetOpeAbleFlg		() { return m_openAbleFlg; }
	const bool& GetOpenFlg			() { return m_openFlg; }

private:

	std::shared_ptr<KdModelWork>	m_spModel;
	std::weak_ptr<Player>			m_wpPlayer;

	Math::Color						m_debugColor;

	Math::Matrix					m_mTrans;

	Math::Vector3					m_pos;
	Math::Vector3					m_eventPos;

	bool							m_openAbleFlg;
	bool							m_openFlg;
};
