#include "CameraController.h"
#include "../../GameObjects/AGameObject.h"
#include "../../../WindowSystem/Keyboard.h"


CameraController::CameraController(AGameObject* freeCam, AGameObject* pawn) : AComponent("CamController", EComponentTypes::Script), freeCam(freeCam), pawn(pawn)
{
	activeCam = nullptr;
	isInFreeCam = false;
	this->freeCam->Enabled = false;
	this->pawn->Enabled = false;
}

CameraController::~CameraController()
{

}

void CameraController::Clone(AComponent* copy)
{

}

void CameraController::Perform()
{
	if (Keyboard::IsKeyPressed(VK_SPACE)) 
	{
		isInFreeCam = !isInFreeCam; 
		 
		activeCam->Enabled = !isInFreeCam; 
		freeCam->Enabled = isInFreeCam; 
		pawn->Enabled = isInFreeCam; 

		if (isInFreeCam) 
		{
			freeCam->GetTransform()->Clone(activeCam->GetOwner()->GetTransform()); 
			pawn->GetTransform()->Clone(activeCam->GetOwner()->GetTransform()); 
		}

		return;
	}

	for (int i = 0; i < camerasList.size(); i++)
	{
		if (!isInFreeCam && Keyboard::IsKeyPressed('1' + i) || 
			!isInFreeCam && i == 9 && Keyboard::IsKeyPressed('0'))
		{
			activeCam->Enabled = false;
			activeCam = camerasList[i];
			activeCam->Enabled = true;
			break;
		}
	}
}

void CameraController::AddCamera(Camera* cam)
{
	if (camerasList.size() >= 10) return;

	camerasList.push_back(cam);
	cam->Enabled = (activeCam == nullptr);
	activeCam = (activeCam == nullptr) ? cam : activeCam;
}