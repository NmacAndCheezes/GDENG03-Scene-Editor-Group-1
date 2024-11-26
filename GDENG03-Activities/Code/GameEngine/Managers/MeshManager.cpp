#include "MeshManager.h"
#include "GameEngine/Meshes/CubeMesh.h"
#include "GameEngine/Meshes/SphereMesh.h"
#include "GameEngine/Meshes/QuadMesh.h"
#include "GameEngine/Meshes/CircleMesh.h"
#include "GameEngine/Meshes/PlaneMesh.h"
#include "GameEngine/Meshes/CylinderMesh.h"
#include "GameEngine/Meshes/ConeMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


#pragma region Singleton
MeshManager* MeshManager::sharedInstance = NULL;

MeshManager* MeshManager::GetInstance()
{
	if (sharedInstance == NULL)
	{
		sharedInstance = new MeshManager();
	}

	return sharedInstance;
}
#pragma endregion

bool MeshManager::Release()
{
	for (auto& pair : primitiveVertices)
	{
		pair.second.clear(); 
		pair.second.shrink_to_fit(); 
	}
	primitiveVertices.clear();

	for (auto& pair : nonPrimitiveVertices) 
	{
		pair.second.clear(); 
		pair.second.shrink_to_fit(); 
	}
	nonPrimitiveVertices.clear(); 

	for (auto& pair : primitiveIndices)
	{
		pair.second.clear(); 
		pair.second.shrink_to_fit(); 
	}
	primitiveIndices.clear();

	for (auto& pair : nonPrimitiveIndices)
	{
		pair.second.clear(); 
		pair.second.shrink_to_fit(); 
	}
	nonPrimitiveIndices.clear();

	return true;
}

std::vector<GenericVertexData> MeshManager::GetVertexDataFromMesh(EPrimitiveMeshTypes meshType)
{
	if (primitiveVertices.find(meshType) == primitiveVertices.end())
	{
		if (!LoadPrimitiveMesh(meshType)) return std::vector<GenericVertexData>();
	}

	return primitiveVertices[meshType];
}

std::vector<GenericVertexData> MeshManager::GetVertexDataFromMesh(std::string meshName)
{
	if (nonPrimitiveVertices.find(meshName) == nonPrimitiveVertices.end())
	{
		if (!LoadNonPrimitiveMesh(meshName)) return std::vector<GenericVertexData>();
	}

	return nonPrimitiveVertices[meshName];
}

std::vector<unsigned short> MeshManager::GetIndexDataFromMesh(EPrimitiveMeshTypes meshType)
{
	if (primitiveIndices.find(meshType) == primitiveIndices.end()) 
	{
		if (!LoadPrimitiveMesh(meshType)) return std::vector<unsigned short>();
	}

	return primitiveIndices[meshType]; 
}

std::vector<unsigned short> MeshManager::GetIndexDataFromMesh(std::string meshName)
{
	if (nonPrimitiveIndices.find(meshName) == nonPrimitiveIndices.end()) 
	{
		if (!LoadNonPrimitiveMesh(meshName)) return std::vector<unsigned short>();
	}

	return nonPrimitiveIndices[meshName]; 
}

bool MeshManager::LoadPrimitiveMesh(EPrimitiveMeshTypes meshType)
{
	AMesh* mesh = nullptr;

	switch (meshType)
	{
	case EPrimitiveMeshTypes::Cube:
		mesh = new CubeMesh();
		break;
	case EPrimitiveMeshTypes::Sphere:
		mesh = new SphereMesh();
		break;
	case EPrimitiveMeshTypes::Quad:
		mesh = new QuadMesh();
		break;
	case EPrimitiveMeshTypes::Circle:
		mesh = new CircleMesh();
		break;
	case EPrimitiveMeshTypes::Plane:
		mesh = new PlaneMesh();
		break;
	case EPrimitiveMeshTypes::Cylinder:
		mesh = new CylinderMesh();
		break;
	case EPrimitiveMeshTypes::Cone:
		mesh = new ConeMesh();
		break;
	default:
		return false;
	}

	std::vector<Vector3> vertexPosition = mesh->GetVertexPositions();
	// add normals
	// add uvs
	std::vector<GenericVertexData> vertices;

	for (int i = 0; i < vertexPosition.size(); i++)
	{
		GenericVertexData v;
		v.pos = vertexPosition[i];
		v.normals = Vector3::Zero;
		v.uv = Vector2::Zero;
		v.vColor = Vector3::One;
		vertices.push_back(v);
	}

	primitiveVertices[meshType] = vertices;
	primitiveIndices[meshType] = mesh->GetIndices();

	delete mesh; 

	return true;
}

bool MeshManager::LoadNonPrimitiveMesh(std::string meshName)
{
	Assimp::Importer imprtr; 
	const auto model = imprtr.ReadFile((STANDARD_MODEL_PATH + meshName),  
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | 
		aiProcess_OptimizeMeshes | aiProcess_ImproveCacheLocality | aiProcess_ValidateDataStructure | 
		aiProcess_FindInstances | aiProcess_PreTransformVertices | aiProcess_SortByPType | aiProcess_FindDegenerates); 

	if (model == nullptr) { OutputDebugString(imprtr.GetErrorString()); return false; } 

	for (int i = 0; i < model->mNumMeshes; i++)  
	{
		const auto pMesh = model->mMeshes[i]; 

		for (int j = 0; j < pMesh->mNumVertices; j++) 
		{
			GenericVertexData v;
			v.pos = Vector3(pMesh->mVertices[j].x, pMesh->mVertices[j].y, pMesh->mVertices[j].z);
			v.normals = pMesh->HasNormals() ? Vector3(pMesh->mNormals[j].x, pMesh->mNormals[j].y, pMesh->mNormals[j].z) : Vector3::Zero;
			v.uv = pMesh->HasTextureCoords(0) ? Vector2(pMesh->mTextureCoords[0][j].x, pMesh->mTextureCoords[0][j].y) : Vector2::Zero;
			v.vColor = pMesh->HasVertexColors(0) ? Vector3(pMesh->mColors[0][j].r, pMesh->mColors[0][j].g, pMesh->mColors[0][j].b) : Vector3::One;
			nonPrimitiveVertices[meshName].push_back(v); 
		}

		for (int j = 0; j < pMesh->mNumFaces; j++)
		{
			for (int k = 0; k < pMesh->mFaces[j].mNumIndices; k++)
			{
				nonPrimitiveIndices[meshName].push_back(pMesh->mFaces[j].mIndices[k]); 
			}
		}
	}

	return true;
}