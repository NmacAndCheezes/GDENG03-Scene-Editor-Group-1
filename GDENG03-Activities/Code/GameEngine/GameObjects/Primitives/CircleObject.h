#pragma once
#include "PrimitiveObject.h"

class CircleObject : public PrimitiveObject
{
public:
	CircleObject(std::string name = "CircleObj", bool isRainbowed = false) 
		: PrimitiveObject(name, EPrimitiveMeshTypes::Circle, isRainbowed) {}
	~CircleObject() {}
};