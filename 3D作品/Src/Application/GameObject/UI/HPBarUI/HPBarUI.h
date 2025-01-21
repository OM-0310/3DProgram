#pragma once

#include "../UIBase.h"

class Player_Main;

class HPBarUI : public UIBase
{
public:

	HPBarUI			()				{}
	~HPBarUI		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetPlayer_Main(const std::shared_ptr<Player_Main>& _spPlayer_Main)
	{
		m_wpPlayer_Main = _spPlayer_Main;
	}

private:

	std::weak_ptr<Player_Main> m_wpPlayer_Main;

	Math::Vector2					m_pivot;
};
