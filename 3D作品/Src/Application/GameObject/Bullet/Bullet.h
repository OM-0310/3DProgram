﻿#pragma once

class Bullet : public KdGameObject
{
public:

	Bullet		()				{}
	~Bullet		()	override	{}

	void Init	()	override;
	void Update	()	override;
	void DrawLit()	override;

	void Shot	(const Math::Vector3 _pos, const Math::Vector3 _dir);

private:

	std::shared_ptr<KdModelData>	m_spModel;

	Math::Matrix					m_mRot		= Math::Matrix::Identity;
	Math::Matrix					m_mTrans	= Math::Matrix::Identity;

	Math::Vector3					m_pos		= Math::Vector3::Zero;
	Math::Vector3					m_moveDir	= Math::Vector3::Zero;

	static const int				BULLETPOW	= 1;
	int								m_lifeSpan	= 0;
	float							m_hitArea	= 0.0f;

	const float						m_moveSpeed = 1.0f;
};
