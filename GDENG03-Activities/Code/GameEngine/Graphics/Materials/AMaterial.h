#pragma once
#include "DirectXClasses/Shader.h"


// materials  might require some serialization and other more modules; so for now materials in this project are HARD-CODED into classes
class AMaterial 
{
public:
	AMaterial(LPCWSTR shaderType) : shaderType(shaderType) {}
	~AMaterial() {};

	virtual void Render() = 0;
	LPCWSTR GetShaderType() { return shaderType; };


protected:
	LPCWSTR shaderType;
};