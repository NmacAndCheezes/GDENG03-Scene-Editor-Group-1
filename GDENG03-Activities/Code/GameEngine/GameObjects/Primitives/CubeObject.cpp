#include "CubeObject.h"
#include "../../Managers/GameObjectManager.h"

CubeObject::CubeObject() : AGameObject("CubeObj"), renderer(nullptr)
{

}

CubeObject::CubeObject(std::string name) : AGameObject(name), renderer(nullptr)
{

}

CubeObject::~CubeObject()
{
	if (renderer) renderer->Release();
}

void CubeObject::Initialize()
{
	renderer = new MeshRenderer(L"DefaultShader");
	renderer->LoadPrimitive(EPrimitiveMeshTypes::Cube);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer);

	isInitialized = true;
}