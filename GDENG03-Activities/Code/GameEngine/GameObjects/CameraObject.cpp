#include "CameraObject.h"


CameraObject::CameraObject(int width, int height, bool isEditorObject) : 
	AGameObject("Camera", isEditorObject), screenWidth(width), screenHeight(height)
{

}

CameraObject::CameraObject(std::string name, int width, int height, bool isEditorObject) : 
	AGameObject(name, isEditorObject), screenWidth(width), screenHeight(height)
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