#pragma once
#include "../CharaBase.h"

class TPSCamera;
class WeaponBase;
class Pistol;
class AssaultRifle;

class Player : public CharaBase
{
public:

	Player						()				{}
	~Player						()	override	{ Release(); }

	void Init					()	override;
	void Update					()	override;
	void PostUpdate				()	override;

	void ChanegeAnimation		();	// アニメーション切り替え処理
	void MoveProcess			();	// 移動処理
	void ChanegeViewPointProcess();	// 視点切り替え処理
	void ChanegePostureProcess	();	// 体勢切り替え処理
	void AimProcess				();	// 銃構え処理
	void ShotProcess			();	// 弾発射処理
	void ChanegeWeaponProcess	();	// 武器切り替え処理

									// プレイヤー状態フラグ切り替え
	void PlayerTypeFlagSwitch	(
									bool _idleFlg,		bool _walkFlg,	bool _runFlg,		bool _readyFlg,
									bool _standFlg,		bool _sitFlg,	bool _sitIdleFlg,	bool _sitWalkFlg,
									bool _sitReadyFlg,	bool _creepFlg,	bool _creepIdleFlg,	bool _creepWalkFlg,	bool _creepReadyFlg
								);

	void SetCamera				(const std::weak_ptr<TPSCamera>& camera)
	{
		m_wpCamera = camera;
	}

	void SetWeapon				(const std::weak_ptr<WeaponBase>& weapon)
	{
		m_wpWeapon = weapon;
	}

	enum class PlayerType
	{
		Idle,
		Walk,
		Run,
		Ready,
		Stand,
		Sit,
		Sit_Idle,
		Sit_Walk,
		Sit_Ready,
		Creep,
		Creep_Idle,
		Creep_Walk,
		Creep_Ready
	};

	enum class PostureType
	{
		Stand,		// 直立
		Sit,		// しゃがみ
		Creep		// 匍匐
	};

private:

	void UpdateRotate(const Math::Vector3& srcMoveVec);

	std::weak_ptr<TPSCamera>	m_wpCamera;		// カメラ情報
	std::weak_ptr<WeaponBase>	m_wpWeapon;		// 武器情報
	std::shared_ptr<KdAnimator> m_spAnimator;	// アニメーション

	Math::Matrix				m_mAdjustRotMat;// 補正回転行列

	Math::Vector3				m_moveDir;		// 移動方向
	float						m_moveSpeed;	// 移動速度

	Math::Vector3				m_angle;		// 角度

	bool						m_moveFlg;		// 移動フラグ
	bool						m_shotFlg;		// エイムフラグ
	bool						m_keyFlg;		// キーフラグ
	bool						m_changeKeyFlg;	// 視点切り替え時用キーフラグ
	bool						m_sitKeyFlg;	// しゃがみキーフラグ
	bool						m_creepKeyFlg;	// 匍匐キーフラグ

	//==プレイヤー状態フラグ==========================================================================================================

	bool						m_idleFlg;
	bool						m_walkFlg;
	bool						m_runFlg;
	bool						m_readyFlg;
	bool						m_standFlg;
	bool						m_sitFlg;
	bool						m_sitIdleFlg;
	bool						m_sitWalkFlg;
	bool						m_sitReadyFlg;
	bool						m_creepFlg;
	bool						m_creepIdleFlg;
	bool						m_creepWalkFlg;
	bool						m_creepReadyFlg;

	//================================================================================================================================

	int							m_sitCnt;

	PlayerType					m_pType;
	PostureType					m_posType;
	POINT						m_FixMousePos;	// カメラ回転用マウス座標の原点
};
