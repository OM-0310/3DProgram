#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera		()				{}
	~TPSCamera		()	override	{}

	enum class CameraType
	{
		//Fps,
		TpsR,
		TpsL,
		AimR,
		AimL,
		SitR,
		SitL,
		SitAimR,
		SitAimL,
		CreepR,
		CreepL,
		CreepAimR,
		CreepAimL
	};

	void				Init			()			override;
	void				Update			()			override;

	//void				ChangeFPS		();
	void				ChangeTPSR		();
	void				ChangeTPSL		();
	void				ChangeAimR		();
	void				ChangeAimL		();
	void				ChangeSitR		();
	void				ChangeSitL		();
	void				ChangeSitAimR	();
	void				ChangeSitAimL	();

	const CameraType	GetPastCamType	()	const					{ return m_pastCamType; }
	const CameraType	GetCamType		()	const					{ return m_camType; }

private:

	const Math::Vector3 m_aimRBasePoint = { 0.7f,1.75f,-0.8f };
	const Math::Vector3 m_aimLBasePoint	= { -0.7f,1.75f,-0.8f };
	//const Math::Vector3 m_fpsBasePoint	= { 0.f, 1.7f, -0.8f };
	const Math::Vector3 m_tpsRBasePoint	= { 0.5f, 1.75f, -3.f };
	const Math::Vector3 m_tpsLBasePoint	= { -0.5f, 1.75f, -3.f };
	const Math::Vector3 m_sitRBasePoint = { 0.5f,1.45f,-3.f };
	const Math::Vector3 m_sitLBasePoint = { -0.5f,1.45f,-3.f };
	const Math::Vector3 m_sitAimRPoint	= { 0.7f, 1.25f, -0.8f };
	const Math::Vector3 m_sitAimLPoint	= { -0.7f,1.25f,-0.8f };

	Math::Vector3		m_basePoint		= Math::Vector3::Zero;

	CameraType			m_camType		= CameraType::TpsL;
	CameraType			m_pastCamType	= CameraType::TpsL;
};