#include "ModelObject.h"
#include "GameEngine/Managers/GameObjectManager.h"
#include "GameEngine/Graphics/Materials/LitTextureMaterial.h"


ModelObject::ModelObject(std::string modelName, std::string textureName) : AGameObject(modelName.substr(0, modelName.size() - 4)),  
	modelName(modelName), textureName(textureName), isRainbowed(false), renderer(nullptr) 
{

}

ModelObject::ModelObject(std::string modelName) : AGameObject(modelName.substr(0, modelName.size() - 4)),
	modelName(modelName), textureName(""), isRainbowed(true), renderer(nullptr)
{
}

ModelObject::~ModelObject()
{
	//if (renderer) renderer->Release();
}

void ModelObject::Initialize()
{
	if (isRainbowed) 
	{
		renderer = new MeshRenderer(); 
	}
	else
	{
		LitTextureMaterial* mat = new LitTextureMaterial(textureName); 
		renderer = new MeshRenderer(mat); 
	}

	renderer->LoadNonPrimitive(modelName, isRainbowed);
	AttachComponent(renderer);
	GameObjectManager::GetInstance()->BindRendererToShader(renderer); 

	isInitialized = true;
}