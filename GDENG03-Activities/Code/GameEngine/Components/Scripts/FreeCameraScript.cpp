#include "FreeCameraScript.h"
#include "../../../WindowSystem/Keyboard.h"
#include "../../../WindowSystem/Mouse.h"
#include "../../MathUtils.h"
#include "../Transform.h"


FreeCameraScript::FreeCameraScript(int width, int height) : AComponent("FreeCamera", EComponentTypes::Script)
{
	oldMousePos = { -1, -1 };
	currMousePos = { -1, -1 };

	yawRotationRate = 180.f / ((float)width * 1.05f); 
	pitchRotationRate = 90.f / ((float)height * 1.15f); 
}

FreeCameraScript::~FreeCameraScript() 
{

}

void FreeCameraScript::Clone(AComponent* copy)
{
	FreeCameraScript* copyCamera = (FreeCameraScript*)copy; 
	if (copyCamera == nullptr) return;

	oldMousePos = copyCamera->oldMousePos; 
	transform->Clone(copyCamera->transform);
}

void FreeCameraScript::Perform()
{
	if (Mouse::IsButtonDown(Mouse::EMouseButtons::Right))
	{
		HandleMovement(); 
		HandleLookRotation();
	}
	else
	{
		HandleWheelInputs();

		oldMousePos = { -1, -1 };
		currMousePos = { -1, -1 };
	}
}

void FreeCameraScript::HandleMovement()
{
	Vector3 total = Vector3::Zero;
	if (Keyboard::IsKeyDown('W')) total += transform->GetLocalForward();
	if (Keyboard::IsKeyDown('S')) total -= transform->GetLocalForward();
	if (Keyboard::IsKeyDown('A')) total -= transform->GetLocalRight(); 
	if (Keyboard::IsKeyDown('D')) total += transform->GetLocalRight(); 
	if (Keyboard::IsKeyDown('E')) total += Vector3::Up; 
	if (Keyboard::IsKeyDown('Q')) total -= Vector3::Up; 
	total.Normalize();
	total *= movementSpeed * dt; 

	if (Keyboard::IsKeyDown(VK_SHIFT)) total *= boostSpeed;

	transform->Position += total;
}

void FreeCameraScript::HandleLookRotation()
{
	oldMousePos = currMousePos;
	currMousePos = Mouse::GetMousePos();

	if (oldMousePos.x == -1 && oldMousePos.y == -1) return;

	Vector2 dir = currMousePos - oldMousePos;

	float x = dir.y * pitchRotationRate;
	if (!MathUtils::IsWithinRange(x + transform->GetEulerAngles().x, -pitchThreshold, pitchThreshold)) x = 0;
	transform->Rotate(x, 0.0f, 0.0f);

	float y = dir.x * yawRotationRate;
	transform->Rotate(0.0f, y, 0.0f); 
}

void FreeCameraScript::HandleWheelInputs()
{
	int rot = Mouse::GetMouseWheelRotations(); 
	transform->Position += transform->GetLocalForward() * longitudinalStepDistance * (float)rot;
}