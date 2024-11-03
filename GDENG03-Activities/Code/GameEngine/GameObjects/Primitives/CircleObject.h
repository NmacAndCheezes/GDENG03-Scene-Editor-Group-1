#pragma once
#include "../AGameObject.h"
#include "../../Components/Renderer/MeshRenderer.h"

class CircleObject : public AGameObject
{
public:
	CircleObject();
	CircleObject(std::string name);
	~CircleObject();
	void Initialize() override;


private:
	MeshRenderer* renderer;
};