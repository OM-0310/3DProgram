#pragma once

#include "../UIBase.h"

class TPSCamera;

class Reticle : public UIBase
{
public:

	Reticle			()				{}
	~Reticle		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void DrawSprite	()	override;

	void SetActive	(bool _Active)	{ m_bitsEachFlg[ActiveFlg] = _Active; }

	void SetTPSCamera(const std::shared_ptr<TPSCamera>& _spTPSCamera)
	{
		m_wpTPSCamera = _spTPSCamera;
	}

private:

	enum
	{
		ActiveFlg
	};

private:

	std::weak_ptr<TPSCamera>	m_wpTPSCamera;

	const Math::Vector2			m_aimRPos = { -15.0f,0.0f };
	//const Math::Vector2			m_aimRPos = Math::Vector2::Zero;
	const Math::Vector2			m_aimLPos = { 18.5f,0.0f };
	//const Math::Vector2			m_aimLPos = Math::Vector2::Zero;

	static constexpr short		m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg> m_bitsEachFlg;
};
