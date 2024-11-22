#pragma once
#include "PrimitiveObject.h"

class SphereObject : public PrimitiveObject 
{
public:
	SphereObject(std::string name = "SphereObj", bool isRainbowed = false) 
		: PrimitiveObject(name, EPrimitiveMeshTypes::Sphere, isRainbowed) {}
	~SphereObject() {}
};