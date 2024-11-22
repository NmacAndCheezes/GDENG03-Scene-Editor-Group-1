#pragma once
#include "../AGameObject.h"
#include "GameEngine/Components/Renderer/MeshRenderer.h"


class PrimitiveObject : public AGameObject
{
public:
	PrimitiveObject(std::string name, EPrimitiveMeshTypes type, bool isRainbowed);
	~PrimitiveObject();
	void Initialize() override;


private:
	MeshRenderer* renderer;
	EPrimitiveMeshTypes type;
	bool isRainbowed;
};