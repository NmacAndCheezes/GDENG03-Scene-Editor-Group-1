#pragma once
#include "../AGameObject.h"
#include "../../Components/Renderer/MeshRenderer.h"

class CylinderObject : public AGameObject
{
public:
	CylinderObject();
	CylinderObject(std::string name);
	~CylinderObject();
	void Initialize() override;


private:
	MeshRenderer* renderer;
};