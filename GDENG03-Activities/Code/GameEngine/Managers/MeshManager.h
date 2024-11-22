#pragma once
#include <SimpleMath.h>
#include <unordered_map>
#include <string>
#include "../Meshes/EPrimitiveMeshTypes.h"
#include "DirectXClasses/Buffers/BufferDataTypes.h"


using namespace DirectX::SimpleMath;

class MeshManager
{
public:
	static MeshManager* GetInstance();
	MeshManager(const MeshManager&) = delete;
	MeshManager& operator=(const MeshManager&) = delete;

	bool Release();

	std::vector<GenericVertexData> GetVertexDataFromMesh(EPrimitiveMeshTypes meshType);
	std::vector<GenericVertexData> GetVertexDataFromMesh(std::string meshName);
	std::vector<unsigned short> GetIndexDataFromMesh(EPrimitiveMeshTypes meshType);
	std::vector<unsigned short> GetIndexDataFromMesh(std::string meshName);

private:
	MeshManager() {};
	~MeshManager() {};

	bool LoadPrimitiveMesh(EPrimitiveMeshTypes meshType);
	bool LoadNonPrimitiveMesh(std::string meshName);


private:
	static MeshManager* sharedInstance;

	const std::string STANDARD_MODEL_PATH = "Assets/Models/";

	std::unordered_map<EPrimitiveMeshTypes, std::vector<GenericVertexData>> primitiveVertices;
	std::unordered_map<std::string, std::vector<GenericVertexData>> nonPrimitiveVertices;

	std::unordered_map<EPrimitiveMeshTypes, std::vector<unsigned short>> primitiveIndices;
	std::unordered_map<std::string, std::vector<unsigned short>> nonPrimitiveIndices;
};