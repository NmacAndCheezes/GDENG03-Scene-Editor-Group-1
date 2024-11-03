#include "Camera.h"
#include "../Transform.h"
#include "../../Graphics/GraphicsEngine.h"
#include "../../Graphics/CameraManager.h"


Camera::Camera(int width, int height) : AComponent("Camera", EComponentTypes::Camera)
{
	ProjectionType = EProjectionTypes::Perspective;
	AspectRatio = Vector2((float)width, (float)height);
	FieldOfView = 60.f;
	clippingPlane = Vector2(0.1f, 1000.f);

	vpMatrixBuffer = new VertexConstantBuffer<VPMatrix>(GraphicsEngine::GetInstance(), 1u);
	vpMatrixBuffer->Init();

	CameraManager::GetInstance()->AddCamera(this);
}

Camera::Camera(std::string name, int width, int height) : AComponent(name, EComponentTypes::Camera)
{
	ProjectionType = EProjectionTypes::Perspective; 
	AspectRatio = Vector2((float)width, (float)height);
	FieldOfView = 60.f; 
	clippingPlane = Vector2(0.1f, 1000.f); 

	vpMatrixBuffer = new VertexConstantBuffer<VPMatrix>(GraphicsEngine::GetInstance(), 1u);
	vpMatrixBuffer->Init();

	CameraManager::GetInstance()->AddCamera(this); 
}

Camera::~Camera()
{
	
}

bool Camera::Release()
{
	if (vpMatrixBuffer)
	{
		vpMatrixBuffer->Release();
	}

	CameraManager::GetInstance()->RemoveCamera(this); 

	return true;
}

void Camera::Clone(AComponent* copy)
{
	Camera* copyCamera = (Camera*)copy;
	if (copyCamera == nullptr) return;

	//Release();
	ProjectionType = copyCamera->ProjectionType;
	AspectRatio = copyCamera->AspectRatio;
	FieldOfView = copyCamera->FieldOfView;
	clippingPlane = copyCamera->clippingPlane;
	//vpMatrixBuffer = copyCamera->vpMatrixBuffer;
}

void Camera::Perform()
{

}

void Camera::BindVPMatrixToPipeline()
{
	if(transform == nullptr) return;

	// note: comment out EVERY DirectXTK Matrix functions for the view and projection matrices, it doesnt work in the shader

	// https://gamedev.stackexchange.com/questions/178643/the-view-matrix-finally-explained
	//Matrix view = Matrix::CreateLookAt(transform->Position, transform->Position + transform->GetLocalForward(), transform->GetLocalUp());
	Matrix view = DirectX::XMMatrixLookAtLH(transform->Position, transform->Position + transform->GetLocalForward(), transform->GetLocalUp());

	Matrix projection = Matrix::Identity;
	if (ProjectionType == EProjectionTypes::Orthographic)
	{
		//projection = Matrix::CreateOrthographic(AspectRatio.x, AspectRatio.y, clippingPlane.x, clippingPlane.y);
		projection = DirectX::XMMatrixOrthographicLH(AspectRatio.x, AspectRatio.y, clippingPlane.x, clippingPlane.y);
	}
	else if (ProjectionType == EProjectionTypes::Perspective)
	{
		//projection = Matrix::CreatePerspectiveFieldOfView(DirectX::XMConvertToRadians(FieldOfView), AspectRatio.x / AspectRatio.y, clippingPlane.x, clippingPlane.y);
		projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(FieldOfView), AspectRatio.x / AspectRatio.y, clippingPlane.x, clippingPlane.y); 
	}

	VPMatrix vpMatrix = { view, projection };
	

	vpMatrixBuffer->SetConstants(vpMatrix); 
	vpMatrixBuffer->BindToPipeline(); 
}

Vector2 Camera::GetClippingPlane()
{
	return clippingPlane;
}

void Camera::SetClippingPlane(float nearZ, float farZ)
{
	clippingPlane = Vector2(nearZ, farZ);
}