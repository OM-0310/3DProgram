#pragma once

class CameraBase :public KdGameObject
{
public:

	CameraBase()						{}
	virtual ~CameraBase()	override	{}

	void Init()				override;
	void Update()			override;
	void PreDraw()			override;

private:
};
