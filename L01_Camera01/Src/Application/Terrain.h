#pragma once

class Terrain : public KdGameObject
{
public:
	Terrain();
	~Terrain()override;

	void Init()override;
	void DrawLit()override;

private:

	// 地形データ
	std::shared_ptr<KdModelData>		m_spModel = nullptr;

	void Release() {}
};
