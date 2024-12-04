#include "SceneManager.h"
#include "GameObjectManager.h"
#include "PhysicsEngine.h"

#include <sstream>
#include <fstream>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>

#include "GameEngine/Graphics/Materials/UnlitRainbowMaterial.h"
#include "GameEngine/Graphics/Materials/UnlitColorMaterial.h"
#include "GameEngine/Graphics/Materials/LitTextureMaterial.h"
#include "GameEngine/GameObjects/EmptyGameObject.h"
#include <GameEngine/Debug.h>


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

	UpdateActiveScene(scenePath); 

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
		if (!rootObj->IsEditorObject()) SaveObject(rootObj, objJSONList, allocator);
	}
	doc.AddMember("GameObjects", objJSONList, allocator);

	FILE* file = fopen(scenePath.c_str(), "wb");
	assert(file != NULL);

	char writeBuffer[16384];
	rapidjson::FileWriteStream os(file, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os); 

	doc.Accept(writer);   
	fclose(file);
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
		if (!child->IsEditorObject()) SaveObject(child, jsonChildList, allocator);
	}
	jsonObj.AddMember("ChildrenList", jsonChildList, allocator);

	rapidjson::Value jsonCompList = rapidjson::Value(rapidjson::kArrayType);
	std::vector<AComponent*> compList = obj->GetAllComponents(); 
	for (auto& comp : compList) 
	{
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

	if (!jsonComp.ObjectEmpty()) compList.PushBack(jsonComp, allocator);
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

	jsonComp.AddMember("Material", jsonMat, allocator);
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


#pragma region Open
void SceneManager::OpenScene(std::string scenePath)
{
	if (scenePath == "") return;

	UpdateActiveScene(scenePath);

	GameObjectManager::DeleteScene();
	PhysicsEngine::GetInstance()->Release();
	PhysicsEngine::GetInstance()->Init(); 

	FILE* inFile = fopen(scenePath.c_str(), "rb");
	assert(inFile != NULL); 

	char readBuffer[16384];
	rapidjson::FileReadStream jsonFile(inFile, readBuffer, sizeof(readBuffer)); 
	rapidjson::Document doc; 

	doc.ParseStream(jsonFile); 
	fclose(inFile); 

	activeScene = doc["SceneName"].GetString();
	for (rapidjson::Value::ConstValueIterator itr = doc["GameObjects"].Begin(); 
		itr != doc["GameObjects"].End(); ++itr)
	{
		InitializeObj(itr, nullptr);
	}
}

void SceneManager::OpenSimpleScene(std::string scenePath)
{
	if (scenePath == "") return;

	UpdateActiveScene(scenePath);

	GameObjectManager::DeleteScene();
	PhysicsEngine::GetInstance()->Release();
	PhysicsEngine::GetInstance()->Init();

	FILE* inFile = fopen(scenePath.c_str(), "rb");
	assert(inFile != NULL);

	char readBuffer[16384];
	rapidjson::FileReadStream jsonFile(inFile, readBuffer, sizeof(readBuffer));
	rapidjson::Document doc;

	doc.ParseStream(jsonFile);
	fclose(inFile);

	activeScene = doc["sceneName"].GetString();
	for (rapidjson::Value::ConstValueIterator itr = doc["GameObjects"].Begin();
		itr != doc["GameObjects"].End(); ++itr)
	{
		// Create a new game object with the name from the JSON
		AGameObject* obj = new AGameObject(itr->GetObj()["Name"].GetString());
		MeshRenderer* rend = new MeshRenderer();

		std::vector<Vector3> vertices;  // Store vertices
		std::vector<unsigned short> indices;  // Store indices (if any)

		// Check if the "vertices" key exists for the current GameObject
		// Iterate over the vertices array of the current GameObject
		for (const auto& vertex : itr->GetObj()["Mesh"].GetObj()["vertices"].GetArray()) {
			// Extract x, y, z components for each vertex
			float x = vertex["x"].GetFloat();
			float y = vertex["y"].GetFloat();
			float z = vertex["z"].GetFloat();
			vertices.push_back(Vector3(x, y, z));

			// Optionally log each vertex
			Debug::Log(std::to_string(x) + " " +
				std::to_string(y) + " " +
				std::to_string(z) + " ");
		}

		for (const auto& index : itr->GetObj()["Mesh"].GetObj()["triangles"].GetArray()) {
			indices.push_back(index.GetInt());
		}

		// You can now load mesh data, render it, or process the vertices further
		// For example:
		rend->LoadUnityMesh(vertices, indices);
		obj->AttachComponent(rend);
		// Add the object to the GameObjectManager
		GameObjectManager::GetInstance()->AddRootObject(obj);
	}
}

void SceneManager::InitializeObj(rapidjson::Value::ConstValueIterator& obj_itr, AGameObject* parent)
{
	EmptyGameObject* newObj = new EmptyGameObject(obj_itr->GetObj()["ObjName"].GetString());
	newObj->Enabled = obj_itr->GetObj()["IsEnabled"].GetBool();

	if (parent) parent->AttachChild(newObj);
	else GameObjectManager::GetInstance()->AddRootObject(newObj);

	for (rapidjson::Value::ConstValueIterator comp_itr = obj_itr->GetObj()["ComponentsList"].Begin();
		comp_itr != obj_itr->GetObj()["ComponentsList"].End(); ++comp_itr)
	{
		std::string compType = comp_itr->GetObj()["ComponentType"].GetString();

		if (compType == "Transform") 
		{
			InitializeTransform(comp_itr, newObj); 
		}
		else if (compType == "MeshRenderer") 
		{
			InitializeMeshRenderer(comp_itr, newObj);
		}
		else if (compType == "RigdBody3D")  
		{
			InitializeRigidBody3D(comp_itr, newObj); 
		}
	}

	for (rapidjson::Value::ConstValueIterator child_itr = obj_itr->GetObj()["ChildrenList"].Begin();
		child_itr != obj_itr->GetObj()["ChildrenList"].End(); ++child_itr)
	{
		InitializeObj(child_itr, newObj);
	}
}

void SceneManager::InitializeTransform(rapidjson::Value::ConstValueIterator& comp_itr, AGameObject* owner)
{
	Vector3 pos = {
		comp_itr->GetObj()["Position"].GetObj()["x"].GetFloat(),
		comp_itr->GetObj()["Position"].GetObj()["y"].GetFloat(),
		comp_itr->GetObj()["Position"].GetObj()["z"].GetFloat()
	};

	Vector3 euler = {
		comp_itr->GetObj()["Rotation"].GetObj()["x"].GetFloat(),
		comp_itr->GetObj()["Rotation"].GetObj()["y"].GetFloat(),
		comp_itr->GetObj()["Rotation"].GetObj()["z"].GetFloat()
	};

	Vector3 scale = {
		comp_itr->GetObj()["Scale"].GetObj()["x"].GetFloat(),
		comp_itr->GetObj()["Scale"].GetObj()["y"].GetFloat(),
		comp_itr->GetObj()["Scale"].GetObj()["z"].GetFloat()
	};

	owner->GetTransform()->Position = pos;
	owner->GetTransform()->Rotate(euler); 
	owner->GetTransform()->LocalScale = scale; 
}

void SceneManager::InitializeMeshRenderer(rapidjson::Value::ConstValueIterator& comp_itr, AGameObject* owner)
{
	MeshRenderer* mr = new MeshRenderer();

	std::string matType = comp_itr->GetObj()["Material"].GetObj()["MaterialType"].GetString();

	if (matType == "RainbowMaterial")
	{
		UnlitRainbowMaterial* mat = new UnlitRainbowMaterial();
		mr->SetMaterial(mat);
	}
	else if (matType == "ColorMaterial")
	{
		UnlitColorMaterial* mat = (UnlitColorMaterial*)mr->GetMaterial();
		Vector3 color = {
			comp_itr->GetObj()["Material"].GetObj()["Color"].GetObj()["r"].GetFloat(),
			comp_itr->GetObj()["Material"].GetObj()["Color"].GetObj()["g"].GetFloat(),
			comp_itr->GetObj()["Material"].GetObj()["Color"].GetObj()["b"].GetFloat()
		};
		mat->SetColor(color);
	}
	else if (matType == "TextureMaterial")
	{
		LitTextureMaterial* mat = new LitTextureMaterial(comp_itr->GetObj()["Material"]["TextureName"].GetString());
		mr->SetMaterial(mat);
	}

	EPrimitiveMeshTypes meshType = (EPrimitiveMeshTypes)comp_itr->GetObj()["MeshType"].GetInt();
	std::string modelName = comp_itr->GetObj()["ModelName"].GetString();
	bool isRainbowed = comp_itr->GetObj()["IsRainbowed"].GetBool();

	if (meshType != EPrimitiveMeshTypes::Unknown)
	{
		mr->LoadPrimitive(meshType, isRainbowed);
	}
	else
	{
		mr->LoadNonPrimitive(modelName, isRainbowed);
	}

	owner->AttachComponent(mr);
	GameObjectManager::GetInstance()->BindRendererToShader(mr);
}

void SceneManager::InitializeRigidBody3D(rapidjson::Value::ConstValueIterator& comp_itr, AGameObject* owner)
{
	RigidBody3D* rb = new RigidBody3D((EPrimitiveMeshTypes)comp_itr->GetObj()["MeshType"].GetInt());
	owner->AttachComponent(rb);
	PhysicsEngine::GetInstance()->RegisterRigidBody(rb);
	rb->UpdateTransform();

	rb->Mass = comp_itr->GetObj()["Mass"].GetFloat();
	rb->BodyType = (rp3d::BodyType)comp_itr->GetObj()["BodyType"].GetInt();
	rb->GravityEnabled = comp_itr->GetObj()["GravityEnabled"].GetBool();
	rb->LinearDamping = comp_itr->GetObj()["LinearDamping"].GetFloat();
	rb->AngularDamping = comp_itr->GetObj()["AngularDamping"].GetFloat();
}
#pragma endregion

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