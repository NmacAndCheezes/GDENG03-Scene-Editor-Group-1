#include "PrimitiveObject.h"
#include "GameEngine/Managers/GameObjectManager.h"
#include "GameEngine/Graphics/Materials/UnlitRainbowMaterial.h"
#include <GameEngine/Graphics/Materials/LitTextureMaterial.h>


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
		LitTextureMaterial* mat = new LitTextureMaterial("blue.jpg");
		renderer = new MeshRenderer(mat);
	}

	renderer->LoadPrimitive(type, isRainbowed);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer);

	isInitialized = true;
}