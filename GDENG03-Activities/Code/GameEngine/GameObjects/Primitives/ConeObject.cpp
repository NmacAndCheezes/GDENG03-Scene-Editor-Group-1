#include "ConeObject.h"
#include "../../Managers/GameObjectManager.h"

ConeObject::ConeObject() : AGameObject("ConeObject"), renderer(nullptr)
{

}

ConeObject::ConeObject(std::string name) : AGameObject(name), renderer(nullptr)
{

}

ConeObject::~ConeObject()
{
	if (renderer) renderer->Release();
}

void ConeObject::Initialize()
{
	renderer = new MeshRenderer(L"DefaultShader");
	renderer->LoadPrimitive(EPrimitiveMeshTypes::Cone);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer);

	isInitialized = true;
}