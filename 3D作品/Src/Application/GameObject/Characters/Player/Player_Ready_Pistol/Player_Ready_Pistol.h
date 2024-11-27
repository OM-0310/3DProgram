#pragma once

#include "../../CharaBase.h"

class Player;

class Player_Ready_Pistol : public CharaBase
{
public:
	
	Player_Ready_Pistol	()				{}
	~Player_Ready_Pistol()	override	{}

	void Init			()	override;
	void Update			()	override;
	void PostUpdate		()	override;
	void DrawUnLit		()	override;

	void ShotBullet(const bool rayFlg);

	void ChangeAnimation(const std::string& _name, bool _isLoop = true, float _time = 0.0f)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name), _isLoop, _time);
	}

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Player>		m_wpPlayer;

	Math::Matrix	m_localMuzzleMat = Math::Matrix::Identity;	// 親から銃口へのローカル行列

	const int		m_magazineSize	= 10;		// マガジンサイズ 10
	int				m_bulletNum		= 0;		// 装弾数

	const float		m_shotWaitMax	= 25.0f;
	float			m_shotWait		= 0.0f;
	const float		m_time			= 1.0f;

	bool			m_shotFlg		= false;	// 発射フラグ
	bool			m_rayBulletFlg	= false;	// レイ発射フラグ
};
