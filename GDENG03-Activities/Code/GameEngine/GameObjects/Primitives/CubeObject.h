#pragma once
#include "PrimitiveObject.h"

class CubeObject : public PrimitiveObject
{
public:
	CubeObject(std::string name = "CubeObj", bool isRainbowed = false) 
		: PrimitiveObject(name, EPrimitiveMeshTypes::Cube, isRainbowed) {}
	~CubeObject() {}
};