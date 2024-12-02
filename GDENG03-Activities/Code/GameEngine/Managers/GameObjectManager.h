#pragma once
#include <d3d11.h>
#include <unordered_map>
#include "../GameObjects/AGameObject.h"
#include "../Components/Renderer/ARenderer.h"
#include <map>

// Should only contain base game objects (one's with no parents)
class GameObjectManager
{
public:
	static GameObjectManager* GetInstance();
	static void Destroy(); 
	GameObjectManager(const GameObjectManager&) = delete;
	GameObjectManager& operator=(const GameObjectManager&) = delete;

	static void DeleteScene();

	// Game-related methods
	void UpdateEditor(float dt);
	void UpdateGame(float dt);
	void Draw();

	// Object-related methods
	void AddRootObject(AGameObject* gameObject);
	void BindRendererToShader(ARenderer* rendererComponent);
	std::vector<AGameObject*> FindObjectsWithName(std::string name);
	void UpdateObjectWithNewName(AGameObject* gameObject, std::string newName);
	void RemoveObject(AGameObject* gameObject); 
	void RemoveObjectByID(unsigned int id);
	void DeleteObject(AGameObject* gameObject);
	void DeleteObjectByID(unsigned int id);
	std::vector<AGameObject*> GetAllGameObjects();
	std::map<unsigned int, AGameObject*> GetGameObjectMap();
	int GetActiveGameObjectsCount();

private:
	void UnbindRendererWithChildren(AGameObject* obj);


private:
	GameObjectManager() {};
	static GameObjectManager* sharedInstance;

	std::vector<AGameObject*> gameObjectList;
	std::map<unsigned int, AGameObject*> gameObjectMap;
	std::unordered_map<std::string, std::vector<AGameObject*>> objectNameMap; // objs can have same names, so make it a list
	std::unordered_map<LPCWSTR, std::vector<AGameObject*>> shaderToObjectsMap;
};

