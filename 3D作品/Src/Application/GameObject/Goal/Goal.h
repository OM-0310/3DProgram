#pragma once

class Goal : public KdGameObject
{
public:

	Goal		()				{}
	~Goal		()	override	{}

	void Init	()	override;
	void Update	()	override;
	//void DrawLit()override;

	void CanBeGoal() { m_goalAbleFlg = true; }

	const bool& GetClearFlg() { return m_clearFlg; }

private:

	Math::Vector3	m_pos			= Math::Vector3::Zero;

	Math::Color		m_debugColor	= kWhiteColor;

	bool			m_goalAbleFlg	= false;
	bool			m_clearFlg		= false;
};
