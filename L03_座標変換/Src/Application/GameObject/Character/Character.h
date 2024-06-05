#pragma once

class Character : public KdGameObject
{
public:
	Character() {}
	virtual ~Character()	override {}

	void Init()				override;
	void Update()			override;
	void DrawLit()			override;
	void DrawSprite()		override;

	void SetCamera(std::shared_ptr<KdCamera> camera) { m_wpCamera = camera; }

private:
	std::shared_ptr<KdTexture>			m_spTex = nullptr;
	std::shared_ptr<KdSquarePolygon>	m_spPoly = nullptr;
	std::weak_ptr<KdCamera>				m_wpCamera;

	POINT m_mousePos;
};