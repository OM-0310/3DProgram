#pragma once

#include"../BaseScene/BaseScene.h"

class TitleScene : public BaseScene
{
public :

	TitleScene() { Init(); }
	~TitleScene() {}

	std::shared_ptr<KdModelData> GetGroundModel		() { return m_spGroundModel; }
	std::shared_ptr<KdModelData> GetBuildingModel	() { return m_spBuildingModel; }

private :

	void Event() override;
	void Init() override;

	void StageInit(std::atomic<bool>& done);

	std::shared_ptr<KdModelData> m_spGroundModel;
	std::shared_ptr<KdModelData> m_spBuildingModel;

};
