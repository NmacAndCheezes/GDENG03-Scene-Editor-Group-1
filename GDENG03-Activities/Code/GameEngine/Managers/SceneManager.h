#pragma once
#include <unordered_map> 
#include <string>
#include <rapidjson/document.h>

#include "GameEngine/GameObjects/AGameObject.h"
#include "../Components/Renderer/MeshRenderer.h"
#include "../Components/Physics/Rigidbody3D.h"

class SceneManager
{
public:
	static SceneManager* GetInstance();

	void SaveScene(std::string scenePath);
	void OpenScene(std::string scenePath);
	void SaveSimpleScene(std::string scenePath);
	void OpenSimpleScene(std::string scenePath);

	std::string GetActiveSceneName();
	bool IsSceneLoaded(std::string sceneName);

private:
	SceneManager() {};

	void UpdateActiveScene(std::string scenePath);

	void SaveObject(AGameObject* obj, rapidjson::Value& parentObjList, rapidjson::Document::AllocatorType& allocator);
	void SaveComponent(AComponent* comp, rapidjson::Value& compList, rapidjson::Document::AllocatorType& allocator);
	void SaveTransform(Transform* t, rapidjson::Value& jsonComp, rapidjson::Document::AllocatorType& allocator);
	void SaveMeshRenderer(MeshRenderer* mr, rapidjson::Value& jsonComp, rapidjson::Document::AllocatorType& allocator);
	void SaveRigidBody3D(RigidBody3D* rb, rapidjson::Value& jsonComp, rapidjson::Document::AllocatorType& allocator);

	void InitializeObj(rapidjson::Value::ConstValueIterator& obj_itr, AGameObject* parent);
	void InitializeTransform(rapidjson::Value::ConstValueIterator& comp_itr, AGameObject* owner);
	void InitializeMeshRenderer(rapidjson::Value::ConstValueIterator& comp_itr, AGameObject* owner);
	void InitializeRigidBody3D(rapidjson::Value::ConstValueIterator& comp_itr, AGameObject* owner);


private:
	static SceneManager* sharedInstance;

	bool isLoading = false;
	std::string activeScene;
	std::string activeScenePath;
};