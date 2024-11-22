#include "UnlitColorMaterial.h"


UnlitColorMaterial::UnlitColorMaterial(const Vector3& color) : AMaterial(L"UnlitColorShader"), color(color)
{

}

UnlitColorMaterial::~UnlitColorMaterial()
{

}

void UnlitColorMaterial::Render()
{

}

Vector3& UnlitColorMaterial::GetColor()
{
	return color;
}
