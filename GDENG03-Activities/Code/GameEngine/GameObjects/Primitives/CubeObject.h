#pragma once
#include "../AGameObject.h"
#include "../../Components/Renderer/MeshRenderer.h"

class CubeObject : public AGameObject
{
public:
	CubeObject();
	CubeObject(std::string name);
	~CubeObject();
	void Initialize() override;


private:
	MeshRenderer* renderer;
};