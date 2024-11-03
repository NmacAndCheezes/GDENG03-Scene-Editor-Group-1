#pragma once
#include <SimpleMath.h>
#include "../AComponent.h"
#include "EProjectionTypes.h"
#include "../../../DirectXClasses/SwapChain.h"
#include "../../../DirectXClasses/Buffers/BufferDataTypes.h"
#include "../../../DirectXClasses/Buffers/ConstantBuffer.cpp"


using namespace DirectX::SimpleMath;

class Camera : public AComponent
{
public:
	Camera(int width, int height);
	Camera(std::string name, int width, int height);
	~Camera();

	bool Release();
	void Clone(AComponent* copy) override;
	void Perform() override;
	void BindVPMatrixToPipeline();

	Vector2 GetClippingPlane();
	void SetClippingPlane(float nearZ, float farZ);
	__declspec(property(get = GetClippingPlane, put = SetClippingPlane)) Vector2 ClippingPlane;


public:
	EProjectionTypes ProjectionType;
	Vector2 AspectRatio;
	float FieldOfView;

private:
	Vector2 clippingPlane;

	VertexConstantBuffer<VPMatrix>* vpMatrixBuffer;

	//SwapChain* swapChain;
	//Color bgColor;

	// add depth, dictates prioprity between cameras
	// viewport properties
	// clear flags, present skybox, solid color, or nothing as bg (requires swapchain)
};