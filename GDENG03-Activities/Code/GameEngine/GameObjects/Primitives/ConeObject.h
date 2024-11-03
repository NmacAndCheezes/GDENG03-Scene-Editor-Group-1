#pragma once
#include "../AGameObject.h"
#include "../../Components/Renderer/MeshRenderer.h"

class ConeObject : public AGameObject
{
public:
	ConeObject();
	ConeObject(std::string name);
	~ConeObject();
	void Initialize() override;


private:
	MeshRenderer* renderer;
};