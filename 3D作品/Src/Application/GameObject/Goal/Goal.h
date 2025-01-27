#pragma once

class Player;

class Goal : public KdGameObject
{
public:

	Goal			()				{}
	~Goal			()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawUnLit	()	override;

	void CanBeGoal() { m_goalAbleFlg = true; }

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const bool& GetGoalAbleFlg() const { return m_goalAbleFlg; }

private:

	std::shared_ptr<KdSquarePolygon>m_spPoly;
	std::weak_ptr<Player>			m_wpPlayer;

	Math::Matrix					m_mRot			= Math::Matrix::Identity;
	Math::Matrix					m_mTrans		= Math::Matrix::Identity;

	Math::Vector3					m_pos			= Math::Vector3::Zero;

	Math::Color						m_debugColor	= kWhiteColor;

	bool							m_goalAbleFlg	= false;
};
