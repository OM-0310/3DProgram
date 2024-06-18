#pragma once

#define SIZE_NUM	1.0f
#define FLOAT_ZERO	0.0f
#define INT_ZERO	0

class CharaBase :public KdGameObject
{
public:
	CharaBase();
	~CharaBase()override;

	void Update()override;
	void PostUpdate()override;

	void Init()override;
	void DrawLit()override;
	void GenerateDepthMapFromLight()override;

	virtual void Death() = 0;

	void SetHPDec(int _decNum)override;

	int GetHP()override { return m_HP; }
	bool GetAlive() { return m_aliveFlg; }

private:

	void Release();

protected:

	std::shared_ptr<KdSquarePolygon>	m_spPoly;

	Math::Matrix						m_mTrans;
	Math::Matrix						m_mScale;
	Math::Matrix						m_mRot;
	Math::Vector3						m_pos;
	Math::Vector3						m_move;
	Math::Vector3						m_size;
	Math::Color							m_color;

	float								m_sizeX;
	float								m_gravity;
	int									m_invinWait;
	bool								m_aliveFlg;
};
