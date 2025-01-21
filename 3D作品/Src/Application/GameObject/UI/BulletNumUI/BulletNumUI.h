#pragma once

#include "../UIBase.h"

class Player_Ready_Pistol;

class BulletNumUI : public UIBase
{
public:

	BulletNumUI		()				{}
	~BulletNumUI	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetPlayer_Ready_Pistol(const std::shared_ptr<Player_Ready_Pistol>& _spPlayer_Ready_Pistol)
	{
		m_wpPlayer_Ready_Pistol = _spPlayer_Ready_Pistol;
	}

private:

	std::weak_ptr<Player_Ready_Pistol> m_wpPlayer_Ready_Pistol;
};


