#include "PrimitiveObject.h"
#include "GameEngine/Managers/GameObjectManager.h"
#include "GameEngine/Graphics/Materials/UnlitRainbowMaterial.h"


PrimitiveObject::PrimitiveObject(std::string name, EPrimitiveMeshTypes type, bool isRainbowed) 
	: AGameObject(name), type(type), renderer(nullptr), isRainbowed(isRainbowed)
{

}

PrimitiveObject::~PrimitiveObject()
{
	//if (renderer) renderer->Release();
}

void PrimitiveObject::Initialize()
{
	if (!isRainbowed)
	{
		renderer = new MeshRenderer();
	}
	else
	{
		UnlitRainbowMaterial* mat = new UnlitRainbowMaterial();
		renderer = new MeshRenderer(mat);
	}

	renderer->LoadPrimitive(type, isRainbowed);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer);

	isInitialized = true;
}