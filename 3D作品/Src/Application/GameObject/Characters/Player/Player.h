#pragma once
#include "../CharaBase.h"

class TPSCamera;
class WeaponBase;
class Pistol;
class AssaultRifle;

class Player : public CharaBase
{
public:

	Player			()				{}
	~Player			()	override	{ Release(); }

	void Init		()	override;
	void Update		()	override;
	void PostUpdate	()	override;

	void MoveProcess();				// 移動処理
	void ChanegeViewPointProcess();	// 視点切り替え処理
	void AimProcess();				// 銃構え処理
	void ChanegeWeaponProcess();	// 武器切り替え処理

	void SetCamera	(const std::weak_ptr<TPSCamera>& camera)
	{
		m_wpCamera = camera;
	}

	void SetWeapon(const std::weak_ptr<WeaponBase>& weapon)
	{
		m_wpWeapon = weapon;
	}

	const Math::Matrix GetRotationMatrix()const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
			DirectX::XMConvertToRadians(m_degAng.y),
			DirectX::XMConvertToRadians(-m_degAng.x),
			DirectX::XMConvertToRadians(m_degAng.z));
	}

	enum PlayerType
	{
		Idle,
		Run,
	};

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
	std::weak_ptr<WeaponBase>	m_wpWeapon;		// 武器情報
	std::shared_ptr<KdAnimator> m_spAnimator;	// アニメーション

	Math::Vector3				m_degAng;		// 角度

	Math::Vector3				m_moveDir;		// 移動方向
	float						m_moveSpeed;	// 移動速度

	Math::Vector3				m_angle;		// 角度

	bool						m_walkFlg;		// 歩行フラグ
	bool						m_dashFlg;		// 走行フラグ
	bool						m_keyFlg;		// キーフラグ
	bool						m_changeKeyFlg;	// 視点切り替え時用キーフラグ

	bool						m_idleFlg;
	bool						m_runFlg;

	PlayerType					m_pType;
	POINT						m_FixMousePos;	// カメラ回転用マウス座標の原点
};
