#pragma once

class Player;

class SecretFile : public KdGameObject
{
public:

	SecretFile						()				{}
	~SecretFile						()	override	{}

	void Init						()	override;
	void Update						()	override;
	void DrawLit					()	override;
	void DrawBright					()	override;
	void GenerateDepthMapFromLight	()	override;

	void Extinction() { m_isExpired = true; }

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const bool& GetCollectFlg()	const { return m_collectFlg; }

private:

	std::shared_ptr<KdModelData>	m_spModel;
	std::weak_ptr<Player>			m_wpPlayer;

	Math::Matrix					m_mTrans;

	Math::Color						m_color;
	float							m_alpha;
	bool							m_alphaFlg;

	Math::Vector3					m_pos;

	float							m_collectArea;
	bool							m_collectFlg;

	// デバッグ用
	Math::Color						m_debugColor;
};
