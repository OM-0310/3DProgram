#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera		()				{}
	~TPSCamera		()	override	{}

	enum class CameraType
	{
		TpsR,
		TpsL,
		AimR,
		AimL,
	};

	void				Init			()			override;
	void				Update			()			override;

	void				UpdateCollision	();

	void				ChangeTPSR		();
	void				ChangeTPSL		();
	void				ChangeAimR		();
	void				ChangeAimL		();

	const CameraType	GetPastCamType	()	const	{ return m_pastCamType; }
	const CameraType	GetCamType		()	const	{ return m_camType; }

private:

	static constexpr Math::Vector3 m_aimRBasePoint	= { 0.7f,1.75f,-0.8f };
	static constexpr Math::Vector3 m_aimLBasePoint	= { -0.7f,1.75f,-0.8f };
	static constexpr Math::Vector3 m_tpsRBasePoint	= { 0.5f, 1.75f, -3.f };
	static constexpr Math::Vector3 m_tpsLBasePoint	= { -0.5f, 1.75f, -3.f };

	Math::Vector3		m_basePoint		= Math::Vector3::Zero;

	CameraType			m_camType		= CameraType::TpsL;
	CameraType			m_pastCamType	= CameraType::TpsL;
};