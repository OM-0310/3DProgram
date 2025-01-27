#pragma once

class TitleLogo : public KdGameObject
{
public:

	TitleLogo		()				{}
	~TitleLogo		()	override	{}

	void Init		()	override;
	void DrawSprite	()	override;

private:

	std::shared_ptr<KdTexture>	m_spTex;

	Math::Vector2				m_pos = Math::Vector2::Zero;
};
