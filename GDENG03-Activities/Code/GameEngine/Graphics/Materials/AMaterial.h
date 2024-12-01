#pragma once
#include "DirectXClasses/Shader.h"
#include "EditorGUI/IRenderableUI.h"


// materials  might require some serialization and other more modules; so for now materials in this project are HARD-CODED into classes
class AMaterial : public IRenderableUI
{
public:
	AMaterial(LPCWSTR shaderType) : shaderType(shaderType) {}
	~AMaterial() {};

	virtual void Render() = 0;
	virtual void RenderUI() = 0;
	LPCWSTR GetShaderType() { return shaderType; };


protected:
	LPCWSTR shaderType;
};