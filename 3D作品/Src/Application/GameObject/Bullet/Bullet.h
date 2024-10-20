#pragma once

class Bullet : public KdGameObject
{
public:

	Bullet		()				{}
	~Bullet		()	override	{}

	void Init	()	override;
	void Update	()	override;
	void DrawLit()	override;

	void OnHit	() { m_isExpired = true; }
	void Shot	(const Math::Vector3 _pos, const Math::Vector3 _dir);

private:

	std::shared_ptr<KdModelData>	m_spModel;

	Math::Matrix					m_mRot;
	Math::Matrix					m_mTrans;

	Math::Vector3					m_pos;
	Math::Vector3					m_moveDir;

	static const int				BULLETPOW = 1;
	int								m_lifeSpan;
	float							m_hitArea;

	const float						m_moveSpeed = 1.0f;
};
