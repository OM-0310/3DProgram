#pragma once

#include "../UIBase.h"

class SecretFile;

class SecretFileUI : public UIBase
{
public:

	SecretFileUI	()				{}
	~SecretFileUI	()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetSecretFile(const std::shared_ptr<SecretFile>& _spFile)
	{
		m_wpFile = _spFile;
	}

private:

	enum
	{
		UseFlg
	};

private:

	std::weak_ptr<SecretFile>	m_wpFile;

	const short					m_lifeSpanMax	= 35;
	const short					m_lifeSpanMin	= 0;
	short						m_lifeSpan		= 0;

	static constexpr short		m_totalEachFlg	= 1;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;
};
