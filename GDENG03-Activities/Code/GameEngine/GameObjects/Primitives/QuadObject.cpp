#include "QuadObject.h"
#include "../../Managers/GameObjectManager.h"

QuadObject::QuadObject() : AGameObject("QuadObject"), renderer(nullptr)
{

}

QuadObject::QuadObject(std::string name) : AGameObject(name), renderer(nullptr)
{

}

QuadObject::~QuadObject()
{
	if (renderer) renderer->Release();
}

void QuadObject::Initialize()
{
	renderer = new MeshRenderer(L"DefaultShader");
	renderer->LoadPrimitive(EPrimitiveMeshTypes::Quad);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer);

	isInitialized = true;
}