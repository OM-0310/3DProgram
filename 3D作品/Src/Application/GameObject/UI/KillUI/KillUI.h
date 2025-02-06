#pragma once

#include "../UIBase.h"

class Player;
class InterrogationUI;

class KillUI : public UIBase
{
public:

	KillUI			()				{}
	~KillUI			()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetUseFlg(const bool& _useFlg) { m_bitsEachFlg.set(UseFlg, _useFlg); }

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	void SetInterrogationUI(const std::shared_ptr<InterrogationUI>& _spInterrogationUI)
	{
		m_wpInterrogationUI = _spInterrogationUI;
	}

	const bool GetUseFlg() const { return m_bitsEachFlg.test(UseFlg); }

private:

	enum
	{
		UseFlg
	};

private:

	std::weak_ptr<Player>			m_wpPlayer;
	std::weak_ptr<InterrogationUI>	m_wpInterrogationUI;

	static constexpr short			m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg>		m_bitsEachFlg;
	
	static constexpr short			m_lifeSpanMax	= 70;
	static constexpr short			m_lifeSpanMin	= 0;
	short							m_lifeSpan		= 0;
};
