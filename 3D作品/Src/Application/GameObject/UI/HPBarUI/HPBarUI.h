#pragma once

#include "../UIBase.h"

class Player_UpperBody;

class HPBarUI : public UIBase
{
public:

	HPBarUI			()				{}
	~HPBarUI		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetPlayer_UpperBody(const std::shared_ptr<Player_UpperBody>& _spPlayer_Up)
	{
		m_wpPlayer_Up = _spPlayer_Up;
	}

private:

	std::weak_ptr<Player_UpperBody> m_wpPlayer_Up;

	Math::Vector2					m_pivot;
};
