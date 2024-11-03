#include "GameObjectManager.h"
#include "../Graphics/ShaderManager.h"
#include "../Graphics/CameraManager.h"


GameObjectManager* GameObjectManager::sharedInstance = NULL;

GameObjectManager* GameObjectManager::GetInstance()
{
	if (sharedInstance == NULL)
	{
		sharedInstance = new GameObjectManager(); 
	}

	return sharedInstance; 
}


#pragma region Game-related methods
//void GameObjectManager::ProcessInputs(WPARAM wParam, LPARAM lParam)
//{
//	for (size_t i = 0; i < gameObjectList.size(); i++)
//	{
//		gameObjectList[i]->ProcessInputs(wParam, lParam);
//	}
//}

void GameObjectManager::Update(float deltaTime)
{
	for (size_t i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->Update(deltaTime);
	}
}

void GameObjectManager::Draw()
{
	auto shadersList = ShaderManager::GetInstance()->GetShaderProgramsList();
	auto camerasList = CameraManager::GetInstance()->GetCamerasList(); 
	
	for (int i = (int)camerasList.size() - 1; i >= 0; i--)
	{
		if (!camerasList[i]->Enabled || !camerasList[i]->GetOwner()->Enabled) continue; 

		camerasList[i]->BindVPMatrixToPipeline(); 

		for (size_t j = 0; j < shadersList.size(); j++) 
		{
			shadersList[j].vShader->BindToPipeline(); 
			shadersList[j].pShader->BindToPipeline(); 

			LPCWSTR shaderType = shadersList[j].shaderType;  
			auto& objectsList = shaderToObjectsMap[shaderType];  

			for (size_t k = 0; k < objectsList.size(); k++)
			{
				if (objectsList[k] == nullptr)
				{
					objectsList.erase(objectsList.begin() + k);
					objectsList.shrink_to_fit();
					continue;
				}

				objectsList[k]->Draw();
			}
		}
	}

	/*for (size_t i = 0; i < shadersList.size(); i++)
	{
		shadersList[i].vShader->BindToPipeline();
		shadersList[i].pShader->BindToPipeline();

		LPCWSTR shaderType = shadersList[i].shaderType;
		auto& objectsList = shaderToObjectsMap[shaderType];
		auto camerasList = CameraManager::GetInstance()->GetCamerasList();

		for (int j = (int)camerasList.size() - 1; j >= 0; j--)
		{
			if (!camerasList[j]->Enabled || !camerasList[j]->GetOwner()->Enabled) continue;

			camerasList[j]->BindVPMatrixToPipeline();

			for (size_t k = 0; k < objectsList.size(); k++)
			{
				if (objectsList[k] == nullptr)
				{
					objectsList.erase(objectsList.begin() + k);
					objectsList.shrink_to_fit();
					continue;
				}

				objectsList[k]->Draw();
			}
		}
	}*/
}
#pragma endregion


#pragma region Object-related methods
void GameObjectManager::AddObject(AGameObject* gameObject)
{
	if (gameObject == nullptr) return;

	if (gameObject->GetParent() != nullptr)
	{
		gameObject->GetParent()->DetachChild(gameObject);
	}

	gameObjectMap[gameObject->GetName()] = gameObject; 
	gameObjectList.push_back(gameObject); 
	if (!gameObject->IsInitialized()) gameObject->Initialize(); 
}

void GameObjectManager::BindRendererToShader(ARenderer* rendererComponent)
{
	LPCWSTR shaderType = rendererComponent->GetShaderType();
	shaderToObjectsMap[shaderType].push_back(rendererComponent->GetOwner());
}

AGameObject* GameObjectManager::FindObjectByName(std::string name)
{
	if (gameObjectMap[name] != NULL)
	{
		return gameObjectMap[name];
	}
	else
	{
		//std::cout << "Object " << name << " not found!" << std::endl;
		return NULL;
	}
}

void GameObjectManager::DeleteObject(AGameObject* gameObject)
{
	if (gameObject == nullptr) return;

	// Detach from the parent, extra safety check
	AGameObject* parent = gameObject->GetParent(); 
	if (parent) parent->DetachChild(gameObject);

	// remove from game object trackers
	RemoveObject(gameObject);

	// remove from shader trackers
	UnbindRendererWithChildren(gameObject);

	// delete
	delete gameObject;
}

void GameObjectManager::DeleteObjectByName(std::string name)
{
	AGameObject* object = FindObjectByName(name);

	if (object != NULL)
	{
		DeleteObject(object); 
	}
}

std::vector<AGameObject*> GameObjectManager::GetAllObjects()
{
	return gameObjectList;
}

int GameObjectManager::GetActiveObjectsCount()
{
	return (int)gameObjectList.size();
}

void GameObjectManager::UnbindRendererWithChildren(AGameObject* obj)
{
	std::vector<AComponent*> componentList = obj->GetComponentsOfType(EComponentTypes::Renderer); 
	for (int i = 0; i < componentList.size(); i++) 
	{
		ARenderer* renderer = dynamic_cast<ARenderer*>(componentList[i]); 
		if (renderer == nullptr) continue; 

		auto& objectsInShaderList = shaderToObjectsMap[renderer->GetShaderType()]; 
		objectsInShaderList.erase(std::remove(objectsInShaderList.begin(), objectsInShaderList.end(), obj)); 
		objectsInShaderList.shrink_to_fit(); 
	}

	std::vector<AGameObject*> childList = obj->GetChildList();
	for (int i = 0; i < childList.size(); i++) 
	{
		UnbindRendererWithChildren(childList[i]);
	}
}

void GameObjectManager::RemoveObject(AGameObject* gameObject)
{
	if (gameObject == nullptr) return;

	// remove from game object trackers
	std::string key = gameObject->GetName();
	gameObjectMap.erase(key);

	auto itr = std::find(gameObjectList.begin(), gameObjectList.end(), gameObject);
	if (itr != gameObjectList.end()) gameObjectList.erase(itr);
	gameObjectList.shrink_to_fit();
}

void GameObjectManager::RemoveObjectByName(std::string name)
{
	AGameObject* object = FindObjectByName(name); 

	if (object != NULL) 
	{
		RemoveObject(object); 
	}
}
#pragma endregion