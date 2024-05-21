#pragma once

class Sun :public KdGameObject
{
public:

	Sun(){}
	~Sun()override{}

	void Init()override;
	void Update()override;
	void DrawLit()override;

private:

	KdSquarePolygon m_polygon;
	Math::Matrix m_scaleMat;
	Math::Matrix m_rotMat;
	Math::Matrix m_transMat;
	Math::Vector3 m_pos;
};
