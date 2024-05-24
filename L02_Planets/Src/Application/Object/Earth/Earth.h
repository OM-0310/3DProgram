#pragma once

class Application;

class Earth :public KdGameObject
{
public:

	Earth() {}
	~Earth()override {}

	void Init()override;
	void Update()override;
	void DrawLit()override;

	void SetMat(Math::Matrix _mat) { m_sunMat = _mat; }

private:

	KdModelData m_model;
	Math::Matrix m_rotMat;
	Math::Matrix m_transMat;
	Math::Matrix m_sunMat;
	Math::Vector3 m_pos;
};
