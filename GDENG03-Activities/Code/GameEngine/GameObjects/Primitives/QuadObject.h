#pragma once
#include "PrimitiveObject.h"

class QuadObject : public PrimitiveObject
{
public:
	QuadObject(std::string name = "QuadObj", bool isRainbowed = false) 
		: PrimitiveObject(name, EPrimitiveMeshTypes::Quad, isRainbowed) {}
	~QuadObject() {}
};