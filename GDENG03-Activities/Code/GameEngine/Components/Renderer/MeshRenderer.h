#pragma once
#include "ARenderer.h"
#include "GameEngine/Meshes/EPrimitiveMeshTypes.h"

class MeshRenderer : public ARenderer
{
public:
	MeshRenderer();
	MeshRenderer(AMaterial* material);
	~MeshRenderer();

	void LoadPrimitive(EPrimitiveMeshTypes type, bool isRainbowed = false);
	void LoadNonPrimitive(std::string modelName, bool isRainbowed = false);


private:
	const std::string STANDARD_MODEL_PATH = "Assets/Models/";
};