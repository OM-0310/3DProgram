#pragma once

class ResultLogo : public KdGameObject
{
public:

	ResultLogo		()				{}
	~ResultLogo		()	override	{}

	void Init		()	override;
	void DrawSprite	()	override;

private:

	std::shared_ptr<KdTexture>	m_spTex;

	Math::Vector2				m_pos = Math::Vector2::Zero;
};
