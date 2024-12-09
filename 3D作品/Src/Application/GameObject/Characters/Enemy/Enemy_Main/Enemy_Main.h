#pragma once

#include "../../CharaBase.h"

class Enemy;

class Enemy_Main : public CharaBase
{
public:

	Enemy_Main		()				{}
	~Enemy_Main		()	override	{}

	void Init		()	override;
	void Update		()	override;
	void PostUpdate	()	override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	void ChangeAnimation(const std::string& _name, bool _isLoop = true, float _time = 0.0f)
	{
		m_spAnimator->SetAnimation(m_spModel->GetData()->GetAnimation(_name), _isLoop, _time);
	}

	void SetEnemy(const std::shared_ptr<Enemy>& _spEnemy)
	{
		m_wpEnemy = _spEnemy;
	}

private:

	std::shared_ptr<KdAnimator> m_spAnimator;
	std::weak_ptr<Enemy>		m_wpEnemy;
};
