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
	void Reload() { m_nowBullet = m_magazineSize; }

	void ChangeAnimation(const std::string& _name, bool _isLoop = true, float _time = 0.0f)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name), _isLoop, _time);
	}

	void SetPlayer(const std::shared_ptr<Player>& _spPlayer)
	{
		m_wpPlayer = _spPlayer;
	}

	const short& GetNowBullet	() const { return m_nowBullet; }
	const short& GetMagazinSize	() const { return m_magazineSize; }
	const short& GetMagazinEmpty() const { return m_magazineEmpty; }

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Player>		m_wpPlayer;

	Math::Matrix	m_localMuzzleMat = Math::Matrix::Identity;	// 親から銃口へのローカル行列

	const short		m_magazineSize	= 10;		// マガジンサイズ 10
	const short		m_magazineEmpty = 0;		// 残弾 空
	short			m_nowBullet		= 0;		// 装弾数

	const short		m_shotWaitMax	= 25;
	const short		m_shotWaitMin	= 0;
	short			m_shotWait		= 0;

	bool			m_shotFlg		= false;	// 発射フラグ
	bool			m_rayBulletFlg	= false;	// レイ発射フラグ
};
