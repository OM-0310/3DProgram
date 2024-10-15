#pragma once

class Bullet : public KdGameObject
{
public:

	Bullet		()				{}
	~Bullet		()	override	{}

	void Init	()	override;
	void Update	()	override;
	void DrawLit()	override;

	void Shot	(const Math::Vector3 _pos, const Math::Vector3 _dir);

private:

	std::shared_ptr<KdModelData>	m_spModel;

	Math::Matrix					m_mRot;
	Math::Matrix					m_mTrans;

	Math::Vector3					m_pos;
	Math::Vector3					m_moveDir;

	int								m_lifeSpan;

	const float						m_moveSpeed = 1.0f;
};
