#include "CameraObject.h"


CameraObject::CameraObject(int width, int height) : AGameObject("Camera"), screenWidth(width), screenHeight(height)
{

}

CameraObject::CameraObject(std::string name, int width, int height) : AGameObject(name), screenWidth(width), screenHeight(height)
{

}

CameraObject::~CameraObject()
{

}

void CameraObject::Initialize()
{
	cameraComponent = new Camera("CamComponent", screenWidth, screenHeight);
	AttachComponent(cameraComponent);

	isInitialized = true;
}