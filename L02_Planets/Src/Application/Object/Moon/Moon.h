#pragma once

class Apllication;

class Moon :public KdGameObject
{
public:

	Moon() {}
	~Moon()override {}

	void Init()override;
	void Update()override;
	void DrawLit()override;

	void SetMat(Math::Matrix _mat) { m_earthMat = _mat; }

private:

	KdModelData m_model;
	Math::Matrix m_rotMat;
	Math::Matrix m_transMat;
	Math::Matrix m_earthMat;
	Math::Vector3 m_pos;
};
