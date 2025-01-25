#pragma once

#include "../UIBase.h"

class SecretFileLocation;
class CardKeyLocation;

class AnswerUI : public UIBase
{
public:

	AnswerUI		()				{}
	~AnswerUI		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetSecretFileLocation	(const std::shared_ptr<SecretFileLocation>& _spFileLocation)
	{
		m_wpFileLocation = _spFileLocation;
	}

	void SetCardKeyLocation		(const std::shared_ptr<CardKeyLocation>& _spCardLocation)	
	{
		m_wpCardLocation = _spCardLocation;
	}

private:

	std::weak_ptr<SecretFileLocation>	m_wpFileLocation;
	std::weak_ptr<CardKeyLocation>		m_wpCardLocation;

	const short							m_lifeSpanMax	= 60;
	const short							m_lifeSpanMin	= 0;
	short								m_lifeSpan		= 0;
};
