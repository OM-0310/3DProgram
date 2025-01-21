#pragma once

#include "../../CharaBase.h"

class Player;

class Player_Main : public CharaBase
{
public:

	Player_Main		()				{}
	~Player_Main	()	override	{}

	void Init			()	override;
	void Update			()	override;
	void PostUpdate		()	override;

	void ChangeAnimation(const std::string& _name, bool _isLoop = true, float _time = 0.0f)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name), _isLoop, _time);
	}

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const int& GetMaxHP		() const { return m_maxHP; }
	const int& GetHalfHP	() const { return m_halfHP; }
	const int& GetQuaterHP	() const { return m_quaterHP; }

private:

	const int			m_maxHP		= 100;		// 最大HP 100
	const int			m_halfHP	= 50;		// HP半分 50
	const int			m_quaterHP	= 25;		// HP4分の1 25

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Player>		m_wpPlayer;
};
