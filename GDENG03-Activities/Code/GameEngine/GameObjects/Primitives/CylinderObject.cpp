#include "CylinderObject.h"
#include "../../Managers/GameObjectManager.h"

CylinderObject::CylinderObject() : AGameObject("CylinderObject"), renderer(nullptr)
{ 

}

CylinderObject::CylinderObject(std::string name) : AGameObject(name), renderer(nullptr)
{

}

CylinderObject::~CylinderObject()
{
	if (renderer) renderer->Release();
}

void CylinderObject::Initialize()
{
	renderer = new MeshRenderer(L"DefaultShader");
	renderer->LoadPrimitive(EPrimitiveMeshTypes::Cylinder);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer);

	isInitialized = true;
}