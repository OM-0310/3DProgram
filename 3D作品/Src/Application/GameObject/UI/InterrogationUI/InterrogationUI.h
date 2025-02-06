#pragma once

#include "../UIBase.h"

class Player;
class KillUI;

class InterrogationUI : public UIBase
{
public:

	InterrogationUI	()				{}
	~InterrogationUI()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetUseFlg(const bool& _useFlg) { m_bitsEachFlg[UseFlg] = _useFlg; }

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	void SetKillUI(const std::shared_ptr<KillUI>& _spKillUI)
	{
		m_wpKillUI = _spKillUI;
	}

	const bool GetUseFlg() const { return m_bitsEachFlg.test(UseFlg); }

private:

	enum
	{
		UseFlg
	};

private:

	std::weak_ptr<Player>		m_wpPlayer;
	std::weak_ptr<KillUI>		m_wpKillUI;

	static constexpr short		m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;

	const short					m_lifeSpanMax	= 70;
	const short					m_lifeSpanMin	= 0;
	short						m_lifeSpan		= 0;
};
