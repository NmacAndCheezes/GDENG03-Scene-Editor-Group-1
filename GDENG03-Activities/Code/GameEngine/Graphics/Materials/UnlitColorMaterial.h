#pragma once
#include "AMaterial.h"
#include <SimpleMath.h>
#include "GameEngine/MathUtils.h"
#include "DirectXClasses/Buffers/BufferDataTypes.h"
#include "DirectXClasses/Buffers/ConstantBuffer.cpp"

using namespace DirectX::SimpleMath;


class UnlitColorMaterial : public AMaterial
{
public:
	UnlitColorMaterial(const Vector3& color);
	~UnlitColorMaterial();

	void Render() override;
	void RenderUI() override;
	Vector3& GetColor();


private:
	Vector3 color;
	PixelConstantBuffer<UnlitColor>* pixelColorBuffer;
};