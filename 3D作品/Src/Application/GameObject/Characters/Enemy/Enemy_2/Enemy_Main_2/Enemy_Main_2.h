#pragma once

#include "../../../CharaBase.h"

class Enemy_2;

class Enemy_Main_2 : public CharaBase
{
public:

	Enemy_Main_2		()				{}
	~Enemy_Main_2		()	override	{}

	void Init						()	override;
	void Update						()	override;
	void PostUpdate					()	override;
	void DrawLit					()	override;
	void GenerateDepthMapFromLight	()	override;

	void ChangeAnimation(const std::string& _name, bool _isLoop = true, float _time = 0.0f)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name), _isLoop, _time);
	}

	void SetEnemy(const std::shared_ptr<Enemy_2>& _spEnemy)
	{
		m_wpEnemy = _spEnemy;
	}

private:

	enum
	{
		FeedOutFlg
	};

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Enemy_2>		m_wpEnemy;

	static constexpr float		m_alphaSpeed = 0.05f;	// アルファ速度

	static constexpr short		m_totalEachFlg = 1;
	std::bitset<m_totalEachFlg>	m_bitsEachFlg;

	static constexpr short		MAXHP = 1;				// HPの最大値
};
