#pragma once
#include "ARenderer.h"
#include "GameEngine/Meshes/EPrimitiveMeshTypes.h"

class MeshRenderer : public ARenderer
{
public:
	MeshRenderer();
	MeshRenderer(AMaterial* material);
	~MeshRenderer();

	virtual void Initialize() override;

	void RenderUI() override;
	void LoadPrimitive(EPrimitiveMeshTypes type, bool isRainbowed = false);
	void LoadNonPrimitive(std::string modelName, bool isRainbowed = false);
	void LoadUnityMesh(std::vector<GenericVertexData> vertices, std::vector<unsigned short> indicies);
	std::string GetModelName();
	EPrimitiveMeshTypes GetMeshType();
	bool IsRainbowed();


private:
	const std::string STANDARD_MODEL_PATH = "Assets/Models/";
	EPrimitiveMeshTypes type;
	std::string modelName;
	std::string textureName;
	bool isRainbowed;
};