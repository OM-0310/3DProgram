#pragma once
#include "../CameraBase.h"

class TitleCamera : public CameraBase
{
public:

	TitleCamera()						{}
	~TitleCamera()			override	{}

	void Init()				override;
	void Update()			override;
};