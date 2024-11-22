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

void GameObjectManager::Destroy()
{
	if (!sharedInstance) return;

	CameraManager::Destroy();
	ShaderManager::Destroy();

	for (int i = (int)sharedInstance->gameObjectList.size() - 1; i >= 0; i--)
	{
		sharedInstance->DeleteObject(sharedInstance->gameObjectList[i]);
	}

	sharedInstance->gameObjectMap.clear();
	sharedInstance->shaderToObjectsMap.clear();

	delete sharedInstance;
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
	// check for invalid game objects
	if (gameObject == nullptr) return;
	
	// check if game object is already tracked by manager
	auto& namedObjList = gameObjectMap[gameObject->Name];
	for (auto& namedObj : namedObjList)
	{
		if (namedObj->GetInstanceID() == gameObject->GetInstanceID()) return;
	}
	
	// check for relationships, manager only tracks root objs
	if (gameObject->GetParent() != nullptr)
	{
		gameObject->GetParent()->DetachChild(gameObject);
	}

	// set trackers
	gameObjectList.push_back(gameObject);
	gameObjectMap[gameObject->Name].push_back(gameObject); 
	if (!gameObject->IsInitialized()) gameObject->Initialize(); 
}

void GameObjectManager::BindRendererToShader(ARenderer* rendererComponent)
{
	LPCWSTR shaderType = rendererComponent->GetShaderType();
	shaderToObjectsMap[shaderType].push_back(rendererComponent->GetOwner());
}

std::vector<AGameObject*> GameObjectManager::FindObjectsWithName(std::string name)
{
	return gameObjectMap[name];
}

void GameObjectManager::UpdateObjectWithNewName(AGameObject* gameObject, std::string newName)
{
	auto& namedList = gameObjectMap[gameObject->Name];
	for (int i = 0; i < namedList.size(); i++) 
	{
		if (namedList[i]->GetInstanceID() == gameObject->GetInstanceID()) namedList.erase(namedList.begin() + i); 
	}

	if (namedList.size() == 0) gameObjectMap.erase(gameObject->Name);

	gameObjectMap[newName].push_back(gameObject);
}

void GameObjectManager::RemoveObject(AGameObject* gameObject)
{
	if (gameObject == nullptr) return;

	auto itr = std::find(gameObjectList.begin(), gameObjectList.end(), gameObject); 

	if (itr != gameObjectList.end()) 
	{
		gameObjectList.erase(itr); 
		gameObjectList.shrink_to_fit(); 

		auto& namedList = gameObjectMap[gameObject->Name]; 
		for (int i = 0; i < namedList.size(); i++)  
		{
			if (namedList[i]->GetInstanceID() == gameObject->GetInstanceID()) namedList.erase(namedList.begin() + i); 
		}
	}
}

void GameObjectManager::RemoveObjectByID(unsigned int id)
{
	auto itr = std::find_if(gameObjectList.begin(), gameObjectList.end(), [&](AGameObject* obj) {
			return obj->GetInstanceID() == id;
		});

	if (itr != gameObjectList.end()) RemoveObject(*itr);
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

void GameObjectManager::DeleteObjectByID(unsigned int id)
{
	auto itr = std::find_if(gameObjectList.begin(), gameObjectList.end(), [&](AGameObject* obj) {
		return obj->GetInstanceID() == id;
		});

	if (itr != gameObjectList.end()) DeleteObject(*itr);
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
#pragma endregion