#pragma once
#include "PrimitiveObject.h"

class ConeObject : public PrimitiveObject
{
public:
	ConeObject(std::string name = "ConeObj", bool isRainbowed = false) 
		: PrimitiveObject(name, EPrimitiveMeshTypes::Cone, isRainbowed) {}
	~ConeObject() {}
};