#pragma once
#include "../CharaBase.h"

class TPSCamera;

class Player : public CharaBase
{
public:

	Player			()				{}
	~Player			()	override	{ Release(); }

	void Init		()	override;
	void Update		()	override;
	void PostUpdate	()	override;

	void SetCamera	(const std::weak_ptr<TPSCamera>& camera)
	{
		m_wpCamera = camera;
	}

private:

	void UpdateRotate(const Math::Vector3& srcMoveVec);

	// カメラ情報
	std::weak_ptr<TPSCamera>	m_wpCamera;

	Math::Vector3				m_moveDir;
	float						m_moveSpeed;

	Math::Vector3				m_angle;

	bool						m_keyFlg;
};
