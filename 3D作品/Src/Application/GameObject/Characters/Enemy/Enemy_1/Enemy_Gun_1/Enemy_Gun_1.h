#pragma once

#include "../../../CharaBase.h"

class Enemy_1;

class Enemy_Gun_1 : public CharaBase
{
public:

	Enemy_Gun_1()				{}
	~Enemy_Gun_1()	override	{}

	void Init						()	override;
	void Update						()	override;
	void PostUpdate					()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void BulletDec	() { m_nowBullet--; }
	void Reload		() { m_nowBullet = m_magazineSize; }

	void ChangeAnimation(const std::string& _name, bool _isLoop = true, float _time = 0.0f)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name), _isLoop, _time);
	}

	void SetAlpha(const float& _alpha)
	{
		m_alpha = _alpha;
	}

	void SetEnemy(const std::shared_ptr<Enemy_1>& _spEnemy)
	{
		m_wpEnemy = _spEnemy;
	}

	const short& GetNowBullet() { return m_nowBullet; }

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Enemy_1>		m_wpEnemy;

	const float					m_alphaSpeed = 0.05f;	// アルファ速度
	bool						m_feedOutFlg = false;	// フェードアウトフラグ

	const short					m_magazineSize	= 30;
	short						m_nowBullet		= 0;
};
