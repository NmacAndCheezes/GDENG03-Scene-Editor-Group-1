#pragma once
#include "PrimitiveObject.h"

class PlaneObject : public PrimitiveObject
{
public:
	PlaneObject(std::string name = "PlaneObj", bool isRainbowed = false) 
		: PrimitiveObject(name, EPrimitiveMeshTypes::Plane, isRainbowed) {}
	~PlaneObject() {}
};