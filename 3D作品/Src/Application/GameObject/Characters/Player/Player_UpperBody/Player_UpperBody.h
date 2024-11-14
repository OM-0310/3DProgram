#pragma once

#include "../../CharaBase.h"

class Player;

class Player_UpperBody : public CharaBase
{
public:

	Player_UpperBody	()				{}
	~Player_UpperBody	()	override	{}

	void Init			()	override;
	void Update			()	override;
	void PostUpdate		()	override;

	void ChangeAnimation(const std::string& _name)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name));
	}

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Player>		m_wpPlayer;
};
