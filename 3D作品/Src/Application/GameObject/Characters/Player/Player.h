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

	void MoveProcess();			// 移動処理
	void ChanegeViewPointProcess();	// 視点切り替え処理

	void SetCamera	(const std::weak_ptr<TPSCamera>& camera)
	{
		m_wpCamera = camera;
	}

	const Math::Matrix GetRotationMatrix()const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z));
	}

private:

	void UpdateRotateByMouse();

	const Math::Matrix GetRotationYMatrix() const
	{
		return Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_degAng.y));
	}

	const Math::Matrix GetRotationXMatrix() const
	{
		return Math::Matrix::CreateRotationX(
			DirectX::XMConvertToRadians(m_degAng.x));
	}

	const Math::Matrix GetRotationZMatrix() const
	{
		return Math::Matrix::CreateRotationZ(
			DirectX::XMConvertToRadians(m_degAng.z));
	}


private:

	void UpdateRotate(const Math::Vector3& srcMoveVec);

	std::weak_ptr<TPSCamera>	m_wpCamera;		// カメラ情報

	Math::Vector3				m_degAng;		// 角度

	Math::Vector3				m_moveDir;		// 移動方向
	float						m_moveSpeed;	// 移動速度

	Math::Vector3				m_angle;		// 角度

	bool						m_walkFlg;		// 歩行フラグ
	bool						m_dashFlg;		// 走行フラグ
	bool						m_keyFlg;		// キーフラグ

	POINT						m_FixMousePos;	// カメラ回転用マウス座標の原点
};
