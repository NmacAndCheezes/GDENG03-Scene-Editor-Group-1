#include "SceneManager.h"
#include "GameObjectManager.h"
#include <sstream>
#include <fstream>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>

#include "GameEngine/Graphics/Materials/UnlitRainbowMaterial.h"
#include "GameEngine/Graphics/Materials/UnlitColorMaterial.h"
#include "GameEngine/Graphics/Materials/LitTextureMaterial.h"


#pragma region Singleton
SceneManager* SceneManager::sharedInstance = NULL;

SceneManager* SceneManager::GetInstance()
{
	if (sharedInstance == NULL)
	{
		sharedInstance = new SceneManager();
	}

	return sharedInstance;
}
#pragma endregion


#pragma region Save
void SceneManager::SaveScene(std::string scenePath)
{
	if (scenePath == "") scenePath = activeScenePath;

	if (scenePath.empty()) return;

	rapidjson::Document doc;  
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();  

	doc.SetObject();
	rapidjson::Value sceneName;
	sceneName.SetString(activeScene.c_str(), activeScene.length(), allocator);
	doc.AddMember("SceneName", sceneName, allocator);

	rapidjson::Value objJSONList = rapidjson::Value(rapidjson::kArrayType);
	std::vector<AGameObject*> rootObjsList = GameObjectManager::GetInstance()->GetAllGameObjects(); 
	for (auto& rootObj : rootObjsList) 
	{
		SaveObject(rootObj, objJSONList, allocator);
	}
	doc.AddMember("GameObjects", objJSONList, allocator);

	FILE* file = fopen(scenePath.c_str(), "wb");
	char writeBuffer[16384];
	rapidjson::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os); 
	doc.Accept(writer);   
	fclose(file);

	UpdateActiveScene(scenePath);
}

void SceneManager::SaveObject(AGameObject* obj, rapidjson::Value& parentObjList, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value jsonObj = rapidjson::Value(rapidjson::kObjectType);
	
	rapidjson::Value jsonObjName;
	jsonObjName.SetString(obj->Name.c_str(), obj->Name.length(), allocator);
	jsonObj.AddMember("ObjName", jsonObjName, allocator);

	rapidjson::Value jsonObjEnabled = rapidjson::Value(obj->Enabled);
	jsonObj.AddMember("IsEnabled", jsonObjEnabled, allocator);

	rapidjson::Value jsonChildList = rapidjson::Value(rapidjson::kArrayType); 
	std::vector<AGameObject*> childList = obj->GetChildList(); 
	for (auto& child : childList) 
	{
		SaveObject(child, jsonChildList, allocator);
	}
	jsonObj.AddMember("ChildrenList", jsonChildList, allocator);

	rapidjson::Value jsonCompList = rapidjson::Value(rapidjson::kArrayType);
	std::vector<AComponent*> compList = obj->GetAllComponents(); 
	for (auto& comp : compList) 
	{
		OutputDebugString(("\n\nTypes: " + std::to_string(jsonCompList.GetType()) + "\n\n").c_str());
		SaveComponent(comp, jsonCompList, allocator);
	}
	jsonObj.AddMember("ComponentsList", jsonCompList, allocator); 

	parentObjList.PushBack(jsonObj, allocator);
}

void SceneManager::SaveComponent(AComponent* comp, rapidjson::Value& compList, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value jsonComp = rapidjson::Value(rapidjson::kObjectType);
	rapidjson::Value jsomCompType; 
	std::string type = "";

	if (Transform* t = dynamic_cast<Transform*>(comp); t != nullptr)
	{
		type = "Transform";
		jsomCompType.SetString(type.c_str(), type.length(), allocator);
		jsonComp.AddMember("ComponentType", jsomCompType, allocator); 
		SaveTransform(t, jsonComp, allocator);
	}
	else if (MeshRenderer* mr = dynamic_cast<MeshRenderer*>(comp); mr != nullptr) 
	{
		type = "MeshRenderer"; 
		jsomCompType.SetString(type.c_str(), type.length(), allocator); 
		jsonComp.AddMember("ComponentType", jsomCompType, allocator); 
		SaveMeshRenderer(mr, jsonComp, allocator);
	}
	else if (RigidBody3D* rb = dynamic_cast<RigidBody3D*>(comp); rb != nullptr) 
	{
		type = "RigidBody3D";
		jsomCompType.SetString(type.c_str(), type.length(), allocator); 
		jsonComp.AddMember("ComponentType", jsomCompType, allocator); 
		SaveRigidBody3D(rb, jsonComp, allocator);
	}

	compList.PushBack(jsonComp, allocator);
}

void SceneManager::SaveTransform(Transform* t, rapidjson::Value& jsonComp, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value pos = rapidjson::Value(rapidjson::kObjectType); 
	rapidjson::Value euler = rapidjson::Value(rapidjson::kObjectType); 
	rapidjson::Value scale = rapidjson::Value(rapidjson::kObjectType); 

	rapidjson::Value posX = rapidjson::Value(t->Position.x); 
	rapidjson::Value posY = rapidjson::Value(t->Position.y); 
	rapidjson::Value posZ = rapidjson::Value(t->Position.z); 
	rapidjson::Value eulerX = rapidjson::Value(t->GetEulerAngles().x); 
	rapidjson::Value eulerY = rapidjson::Value(t->GetEulerAngles().y); 
	rapidjson::Value eulerZ = rapidjson::Value(t->GetEulerAngles().z); 
	rapidjson::Value scaleX = rapidjson::Value(t->LocalScale.x); 
	rapidjson::Value scaleY = rapidjson::Value(t->LocalScale.y); 
	rapidjson::Value scaleZ = rapidjson::Value(t->LocalScale.z); 

	pos.AddMember("x", posX, allocator); 
	pos.AddMember("y", posY, allocator);  
	pos.AddMember("z", posZ, allocator); 
	euler.AddMember("x", eulerX, allocator); 
	euler.AddMember("y", eulerY, allocator); 
	euler.AddMember("z", eulerZ, allocator); 
	scale.AddMember("x", scaleX, allocator); 
	scale.AddMember("y", scaleY, allocator); 
	scale.AddMember("z", scaleZ, allocator); 

	jsonComp.AddMember("Position", pos, allocator); 
	jsonComp.AddMember("Rotation", euler, allocator); 
	jsonComp.AddMember("Scale", scale, allocator); 
}

void SceneManager::SaveMeshRenderer(MeshRenderer* mr, rapidjson::Value& jsonComp, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value jsonModelName;
	std::string modelName = mr->GetModelName();
	jsonModelName.SetString(modelName.c_str(), modelName.length(), allocator);
	jsonComp.AddMember("ModelName", jsonModelName, allocator);

	rapidjson::Value jsonMeshType = rapidjson::Value((int)mr->GetMeshType());
	jsonComp.AddMember("MeshType", jsonMeshType, allocator);

	rapidjson::Value jsonIsRainbowed = rapidjson::Value(mr->IsRainbowed());
	jsonComp.AddMember("IsRainbowed", jsonIsRainbowed, allocator);

	AMaterial* mat = mr->GetMaterial();
	rapidjson::Value jsonMat = rapidjson::Value(rapidjson::kObjectType); 
	rapidjson::Value jsonMatType;
	std::string matType = ""; 

	if (UnlitRainbowMaterial* rbm = dynamic_cast<UnlitRainbowMaterial*>(mat); rbm != nullptr)
	{
		matType = "RainbowMaterial";
		jsonMatType.SetString(matType.c_str(), matType.length(), allocator); 
		jsonMat.AddMember("MaterialType", jsonMatType, allocator); 
	}
	else if (UnlitColorMaterial* cm = dynamic_cast<UnlitColorMaterial*>(mat); cm != nullptr) 
	{
		matType = "ColorMaterial";
		jsonMatType.SetString(matType.c_str(), matType.length(), allocator);
		jsonMat.AddMember("MaterialType", jsonMatType, allocator);

		rapidjson::Value color = rapidjson::Value(rapidjson::kObjectType);
		rapidjson::Value r = rapidjson::Value(cm->GetColor().x);
		rapidjson::Value g = rapidjson::Value(cm->GetColor().y); 
		rapidjson::Value b = rapidjson::Value(cm->GetColor().z); 
		color.AddMember("r", r, allocator);
		color.AddMember("g", g, allocator);
		color.AddMember("b", b, allocator);
		jsonMat.AddMember("Color", color, allocator);
	}
	else if (LitTextureMaterial* tm = dynamic_cast<LitTextureMaterial*>(mat); tm != nullptr) 
	{
		matType = "TextureMaterial";
		jsonMatType.SetString(matType.c_str(), matType.length(), allocator); 
		jsonMat.AddMember("MaterialType", jsonMatType, allocator);

		rapidjson::Value jsonTextureName;
		jsonTextureName.SetString(tm->GetTextureName().c_str(), tm->GetTextureName().length(), allocator);
		jsonMat.AddMember("TextureName", jsonTextureName, allocator);
	}
}

void SceneManager::SaveRigidBody3D(RigidBody3D* rb, rapidjson::Value& jsonComp, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value jsonMass = rapidjson::Value(rb->Mass);
	jsonComp.AddMember("Mass", jsonMass, allocator);

	rapidjson::Value jsonBodyType = rapidjson::Value((int)rb->BodyType);
	jsonComp.AddMember("BodyType", jsonBodyType, allocator);

	rapidjson::Value jsonGravityEnabled = rapidjson::Value(rb->GravityEnabled);
	jsonComp.AddMember("GravityEnabled", jsonGravityEnabled, allocator);

	rapidjson::Value jsonLinearDamping = rapidjson::Value(rb->LinearDamping);
	jsonComp.AddMember("LinearDamping", jsonLinearDamping, allocator);

	rapidjson::Value jsonAngularDamping = rapidjson::Value(rb->AngularDamping);
	jsonComp.AddMember("AngularDamping", jsonAngularDamping, allocator);

	rapidjson::Value jsonMeshType = rapidjson::Value((int)rb->GetMeshType());
	jsonComp.AddMember("MeshType", jsonMeshType, allocator);
}
#pragma endregion


void SceneManager::OpenScene(std::string scenePath)
{
	toLoadScene = scenePath;

	UnloadActiveScene();

	activeScene = toLoadScene;
	toLoadScene = "";
}

void SceneManager::UnloadActiveScene()
{
	if (activeScene != "")
	{
		
	}
}

std::string SceneManager::GetActiveSceneName()
{
	return activeScene;
}

bool SceneManager::IsSceneLoaded(std::string sceneName)
{
	return activeScene == sceneName;
}

void SceneManager::UpdateActiveScene(std::string scenePath)
{
	activeScenePath = scenePath;

	std::stringstream ss = std::stringstream(scenePath);
	std::vector<std::string> tempList;
	std::string temp;
	while (std::getline(ss, temp, '\\'))
	{
		tempList.push_back(temp);
	}

	ss = std::stringstream(tempList.back());
	tempList.clear(); tempList.shrink_to_fit();
	temp = "";
	while (std::getline(ss, temp, '.'))
	{
		tempList.push_back(temp);
	}

	activeScene = tempList.front();
}