#pragma once
#include "AMaterial.h"
#include <SimpleMath.h>
#include "GameEngine/MathUtils.h"

using namespace DirectX::SimpleMath;


class UnlitRainbowMaterial : public AMaterial
{
public:
	UnlitRainbowMaterial();
	~UnlitRainbowMaterial();

	void Render() override;
	void RenderUI() override;
};