#include "PrimitiveObject.h"
#include "GameEngine/Managers/GameObjectManager.h"


PrimitiveObject::PrimitiveObject(std::string name, EPrimitiveMeshTypes type, bool isRainbowed) 
	: AGameObject(name), type(type), renderer(nullptr), isRainbowed(isRainbowed)
{

}

PrimitiveObject::~PrimitiveObject()
{
	if (renderer) renderer->Release();
}

void PrimitiveObject::Initialize()
{
	renderer = new MeshRenderer();
	renderer->LoadPrimitive(type, isRainbowed);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer);

	isInitialized = true;
}