#pragma once

class TPSCamera;
class Goal;

class GoalUI : public KdGameObject
{
public:

	GoalUI			()				{}
	~GoalUI			()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawUnLit	()	override;

	void SetCamera(const std::shared_ptr<TPSCamera>& _spCamera)
	{
		m_wpCamera = _spCamera;
	}

	void SetGoal(const std::shared_ptr<Goal>& _spGoal)
	{
		m_wpGoal = _spGoal;
	}

private:

	std::shared_ptr<KdSquarePolygon>m_spPoly;
	std::weak_ptr<TPSCamera>		m_wpCamera;
	std::weak_ptr<Goal>				m_wpGoal;
};


