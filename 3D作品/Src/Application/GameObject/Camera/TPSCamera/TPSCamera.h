#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
						TPSCamera		()						{}
						~TPSCamera		()			override	{}

	enum class CameraType
	{
		//Fps,
		TpsR,
		TpsL,
		AimR,
		AimL
	};

	void				Init			()			override;
	void				Update			()			override;

	//void				ChangeFPS		();
	void				ChangeTPSR		();
	void				ChangeTPSL		();
	void				ChangeAimR		();
	void				ChangeAimL		();

	const CameraType	GetPastCamType	()	const					{ return m_pastCamType; }
	const CameraType	GetCamType		()	const					{ return m_camType; }

private:

	const Math::Vector3 m_aimRBasePoint = { 0.5f,1.8f,-1.25f };
	const Math::Vector3 m_aimLBasePoint	= { -0.5f,1.8f,-1.25f };
	//const Math::Vector3 m_fpsBasePoint	= { 0.f, 1.7f, -0.8f };
	const Math::Vector3 m_tpsRBasePoint	= { 0.5f, 1.8f, -2.25f };
	const Math::Vector3 m_tpsLBasePoint	= { -0.5f, 1.8f, -2.25f };

	Math::Vector3		m_basePoint;

	bool				m_fpsFlg;
	bool				m_tpsFlg;

	CameraType			m_camType;
	CameraType			m_pastCamType;
};