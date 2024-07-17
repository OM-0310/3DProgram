#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera				()						{}
	~TPSCamera				()			override	{}

	enum class CameraType
	{
		FPS,
		TPS
	};

	void Init				()			override;
	void Update				()			override;

	void ChangeFPS			()						{ m_camType = CameraType::FPS; }
	void ChangeTPS			()						{ m_camType = CameraType::TPS; }

	CameraType GetCameraType()						{ return m_camType; }

private:

	const Math::Vector3 m_fpsBasePoint = { 0.f, 1.8f, -0.8f };
	const Math::Vector3 m_tpsBasePoint = { 0.f, 1.8f, 3.f };

	Math::Vector3		m_basePoint;

	bool				m_fpsFlg;
	bool				m_tpsFlg;

	CameraType			m_camType;
};