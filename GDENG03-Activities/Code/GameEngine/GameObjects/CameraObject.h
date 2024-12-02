#pragma once
#include "AGameObject.h"
#include "../Components/Camera/Camera.h"


class CameraObject : public AGameObject
{
public:
	CameraObject(int width, int height, bool isEditorObject = false);
	CameraObject(std::string name, int width, int height, bool isEditorObject = false);
	~CameraObject();

	void Initialize() override;


public:
	Camera* cameraComponent;

protected:
	int screenWidth;
	int screenHeight;
};