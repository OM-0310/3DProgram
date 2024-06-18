#pragma once

class UIBase :public KdGameObject
{
public:

	UIBase();
	~UIBase()override;

	void Init()override;
	void Update()override;
	void DrawSprite()override{}
	void DrawLit()override{}

private:

	void Release(){}

protected:

	Math::Matrix						m_mScale;
	Math::Matrix						m_mTrans;

	Math::Color							m_color;

	Math::Vector3						m_pos;
};
