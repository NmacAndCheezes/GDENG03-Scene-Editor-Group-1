#pragma once
#include "PrimitiveObject.h"

class CylinderObject : public PrimitiveObject
{
public:
	CylinderObject(std::string name = "CylinderObj", bool isRainbowed = false) 
		: PrimitiveObject(name, EPrimitiveMeshTypes::Cylinder, isRainbowed) {}
	~CylinderObject() {}
};