#pragma once
#include "AMaterial.h"
#include <SimpleMath.h>
#include "GameEngine/MathUtils.h"

using namespace DirectX::SimpleMath;


class UnlitColorMaterial : public AMaterial
{
public:
	UnlitColorMaterial(const Vector3& color);
	~UnlitColorMaterial();

	void Render() override;
	Vector3& GetColor();


private:
	Vector3 color;
};