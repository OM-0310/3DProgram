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

	std::weak_ptr<SecretFile>	m_wpFile;

	int							m_lifeSpan	= 0;

	bool						m_useFlg	= false;
};
