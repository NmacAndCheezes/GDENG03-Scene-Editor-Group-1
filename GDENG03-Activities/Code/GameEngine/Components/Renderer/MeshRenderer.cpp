#include "MeshRenderer.h"
#include "GameEngine/Managers/MeshManager.h"
#include "DirectXClasses/Buffers/VertexBuffer.cpp"
#include "GameEngine/Graphics/Materials/UnlitColorMaterial.h"
#include "GameEngine/Graphics/Materials/UnlitRainbowMaterial.h"
#include <GameEngine/Debug.h>


MeshRenderer::MeshRenderer() : ARenderer("MeshRenderer")
{
	topologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

MeshRenderer::MeshRenderer(AMaterial* material) : ARenderer("MeshRenderer", material)
{
	topologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::Initialize()
{
	/*if (type != EPrimitiveMeshTypes::Unknown)
		LoadPrimitive(type, isRainbowed);
	else if (modelName != "")
		LoadNonPrimitive(modelName, isRainbowed);*/
}

void MeshRenderer::RenderUI()
{
	material->RenderUI();
}

void MeshRenderer::LoadPrimitive(EPrimitiveMeshTypes type, bool isRainbowed)
{
	this->type = type;
	this->modelName = "";
	this->isRainbowed = isRainbowed;

	auto vertices = MeshManager::GetInstance()->GetVertexDataFromMesh(type);
	auto indices = MeshManager::GetInstance()->GetIndexDataFromMesh(type);

	if (vertices.size() == 0) return;

	InitRenderer();

	if (isRainbowed)
	{
		std::vector<VUnlitRainbowData> verticesWithColor;
		for (auto& v : vertices)
		{
			VUnlitRainbowData vWithColor; 
			vWithColor.pos = v.pos;
			vWithColor.vColor = Vector3(MathUtils::RandFloatWithRange(), MathUtils::RandFloatWithRange(), MathUtils::RandFloatWithRange());
			verticesWithColor.push_back(vWithColor);
			Debug::Log(std::to_string(vWithColor.pos.x));
		}

		VertexBuffer<VUnlitRainbowData>* vb = new VertexBuffer<VUnlitRainbowData>(GraphicsEngine::GetInstance(), verticesWithColor);
		vb->Init();
		vertexBuffer = vb; 
	}
	else
	{
		UnlitColorMaterial* unlit = (UnlitColorMaterial*)material;

		std::vector<VUnlitColorData> verticesWithColor;
		for (auto& v : vertices)
		{
			VUnlitColorData vWithColor;
			vWithColor.pos = v.pos;
			verticesWithColor.push_back(vWithColor);
			Debug::Log(std::to_string(vWithColor.pos.x));
		}

		VertexBuffer<VUnlitColorData>* vb = new VertexBuffer<VUnlitColorData>(GraphicsEngine::GetInstance(), verticesWithColor);
		vb->Init();
		vertexBuffer = vb;
	}

	indexBuffer = new IndexBuffer(GraphicsEngine::GetInstance(), indices);
	indexBuffer->Init();

	mesh = new GenericMesh(vertices, indices);
}

void MeshRenderer::LoadNonPrimitive(std::string modelName, bool isRainbowed)
{
	this->type = EPrimitiveMeshTypes::Unknown;
	this->modelName = modelName;
	this->isRainbowed = isRainbowed; 

	auto vertices = MeshManager::GetInstance()->GetVertexDataFromMesh(modelName); 
	auto indices = MeshManager::GetInstance()->GetIndexDataFromMesh(modelName); 

	if (vertices.size() == 0) return; 

	InitRenderer();

	if (isRainbowed) 
	{
		std::vector<VUnlitRainbowData> verticesWithColor; 
		for (auto& v : vertices)  
		{
			VUnlitRainbowData vWithColor; 
			vWithColor.pos = v.pos;  
			vWithColor.vColor = Vector3(MathUtils::RandFloatWithRange(), MathUtils::RandFloatWithRange(), MathUtils::RandFloatWithRange());   
			verticesWithColor.push_back(vWithColor);
			Debug::Log(std::to_string(vWithColor.pos.x));
		}

		VertexBuffer<VUnlitRainbowData>* vb = new VertexBuffer<VUnlitRainbowData>(GraphicsEngine::GetInstance(), verticesWithColor); 
		vb->Init(); 
		vertexBuffer = vb; 
	}
	else
	{
		std::vector<VLitTextureData> verticesWithUV;
		for (auto& v : vertices)
		{
			VLitTextureData vWithUV; 
			vWithUV.pos = v.pos; 
			vWithUV.normals = v.normals; 
			vWithUV.uv = v.uv;
			verticesWithUV.push_back(vWithUV); 
			Debug::Log(std::to_string(vWithUV.pos.x));
		}

		VertexBuffer<VLitTextureData>* vb = new VertexBuffer<VLitTextureData>(GraphicsEngine::GetInstance(), verticesWithUV);
		vb->Init();
		vertexBuffer = vb;
	}

	indexBuffer = new IndexBuffer(GraphicsEngine::GetInstance(), indices); 
	indexBuffer->Init(); 

	mesh = new GenericMesh(vertices, indices);
}

void MeshRenderer::LoadMeshWithVertexAndIndices(std::vector<GenericVertexData> vertices, std::vector<unsigned short> indices)
{
	this->type = EPrimitiveMeshTypes::Unknown;

	if (vertices.size() == 0) return;

	InitRenderer();

	std::vector<VUnlitRainbowData> verticesWithColor;
	for (auto& v : vertices)
	{
		VUnlitRainbowData vWithColor;
		vWithColor.pos = v.pos;
		vWithColor.vColor = Vector3(MathUtils::RandFloatWithRange(), MathUtils::RandFloatWithRange(), MathUtils::RandFloatWithRange());
		verticesWithColor.push_back(vWithColor);
		Debug::Log(std::to_string(vWithColor.pos.x));
	}

	VertexBuffer<VUnlitRainbowData>* vb = new VertexBuffer<VUnlitRainbowData>(GraphicsEngine::GetInstance(), verticesWithColor);
	vb->Init();
	vertexBuffer = vb;

	indexBuffer = new IndexBuffer(GraphicsEngine::GetInstance(), indices);
	indexBuffer->Init();

	mesh = new GenericMesh(vertices, indices);
}

std::string MeshRenderer::GetModelName()
{
	return modelName;
}

EPrimitiveMeshTypes MeshRenderer::GetMeshType()
{
	return type;
}

GenericMesh* MeshRenderer::GetGenericMesh()
{
	return mesh;
}

bool MeshRenderer::IsRainbowed()
{
	return isRainbowed;
}
