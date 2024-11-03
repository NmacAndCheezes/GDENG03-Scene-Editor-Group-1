#include "CircleObject.h"
#include "../../Managers/GameObjectManager.h"

CircleObject::CircleObject() : AGameObject("CircleObj"), renderer(nullptr)
{

}

CircleObject::CircleObject(std::string name) : AGameObject(name), renderer(nullptr)
{

}

CircleObject::~CircleObject()
{
	if (renderer) renderer->Release();
}

void CircleObject::Initialize()
{
	renderer = new MeshRenderer(L"DefaultShader"); 
	renderer->LoadPrimitive(EPrimitiveMeshTypes::Circle);
	AttachComponent(renderer); 
	GameObjectManager::GetInstance()->BindRendererToShader(renderer); 

	isInitialized = true;
}