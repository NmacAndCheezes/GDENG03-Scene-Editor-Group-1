#include "FreeCameraObject.h"
#include "../Components/Scripts/FreeCameraScript.h"

FreeCameraObject::FreeCameraObject(int width, int height) : CameraObject(width, height)
{

}

FreeCameraObject::FreeCameraObject(std::string name, int width, int height) : CameraObject(name, width, height)
{

}

FreeCameraObject::~FreeCameraObject()
{

}

void FreeCameraObject::Initialize()
{
	FreeCameraScript* script = new FreeCameraScript(screenWidth, screenHeight);
	AttachComponent(script);

	CameraObject::Initialize();
}