#pragma once
#include "AGameObject.h"
#include "../Components/Renderer/MeshRenderer.h"


class ModelObject : public AGameObject
{
public:
	ModelObject(std::string modelName, std::string textureName);
	ModelObject(std::string modelName);
	~ModelObject();

	void Initialize() override;


private:
	std::string modelName;
	std::string textureName;
	bool isRainbowed;
	MeshRenderer* renderer;
};