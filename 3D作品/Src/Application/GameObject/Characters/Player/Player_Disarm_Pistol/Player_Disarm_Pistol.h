#pragma once

#include "../../CharaBase.h"

class Player;

class Player_Disarm_Pistol : public CharaBase
{
public:

	Player_Disarm_Pistol	()				{}
	~Player_Disarm_Pistol	()	override	{}

	void Init				()	override;
	void Update				()	override;
	void PostUpdate			()	override;
	void DrawUnLit			()	override;

	void ChangeAnimation(const std::string& _name, bool _isLoop = true, float _time = 0.0f)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name), _isLoop, _time);
	}

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Player>		m_wpPlayer;
};
