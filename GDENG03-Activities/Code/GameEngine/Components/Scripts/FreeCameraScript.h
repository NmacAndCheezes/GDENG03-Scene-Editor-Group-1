#pragma once
#include "../AComponent.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

class FreeCameraScript : public AComponent
{
public:
	FreeCameraScript(int width, int height);
	~FreeCameraScript();

	void Clone(AComponent* copy) override;
	void Perform() override;

private:
	void HandleMovement();
	void HandleLookRotation();
	void HandleWheelInputs();


private:
	const float movementSpeed = 8.5f;
	const float boostSpeed = 4.7f;
	const float longitudinalStepDistance = 5.0f;
	const float pitchThreshold = 90.0f;

	// camera rotation based on how much mouse pos traversed on the screen; not reliant on dt
	float pitchRotationRate;
	float yawRotationRate;

	Vector2 oldMousePos;
	Vector2 currMousePos;
};