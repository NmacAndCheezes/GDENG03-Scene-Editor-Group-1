#pragma once

#include "AGameObject.h"

// only contains components
class EmptyGameObject : public AGameObject
{
public:
	EmptyGameObject(std::string name);
	~EmptyGameObject();
	void Initialize();
};