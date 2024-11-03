#pragma once
#include "CameraObject.h"


class FreeCameraObject : public CameraObject
{
public:
	FreeCameraObject(int width, int height);
	FreeCameraObject(std::string name, int width, int height);
	~FreeCameraObject();

	void Initialize() override;
};