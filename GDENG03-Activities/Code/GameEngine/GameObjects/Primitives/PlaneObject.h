#pragma once
#include "../AGameObject.h"
#include "../../Components/Renderer/MeshRenderer.h"

class PlaneObject : public AGameObject
{
public:
	PlaneObject();
	PlaneObject(std::string name);
	~PlaneObject();
	void Initialize() override;


private:
	MeshRenderer* renderer;
};