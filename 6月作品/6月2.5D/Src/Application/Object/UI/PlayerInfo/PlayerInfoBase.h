#pragma once
#include "../UIBase.h"

class PlayerInfoBase :public UIBase
{
public:

	PlayerInfoBase(){}
	~PlayerInfoBase()override { Release(); }

	void Init()override;
	void Update()override;
	void DrawSprite()override;

private:

	void Release() { m_spTex = nullptr; }

private:

	std::shared_ptr<KdTexture>	m_spTex;

	Math::Vector2				m_rectPos;
	Math::Rectangle				m_rect;
};