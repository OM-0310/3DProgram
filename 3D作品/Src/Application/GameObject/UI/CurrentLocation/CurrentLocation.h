#pragma once

#include "../UIBase.h"

class Player;

class CurrentLocation : public UIBase
{
public:

	CurrentLocation	()				{}
	~CurrentLocation()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void Open(const bool& _active) { m_bitsEachFlg.set(ActiveFlg, _active); }

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const bool GetActive() const;

private:

	enum
	{
		ActiveFlg
	};

private:

	std::weak_ptr<Player>	m_wpPlayer;					// プレイヤー情報

	static constexpr short	m_activeFlg		= 1;
	std::bitset<m_activeFlg>m_bitsEachFlg;

	static constexpr float	m_posMulti		= 2.5f;		// 座標掛け算値
	static constexpr float	m_adjustZPos	= 157.5f;	// Z座標調整
};
