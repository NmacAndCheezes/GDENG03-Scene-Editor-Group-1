#pragma once
#include <vector>
#include "../AComponent.h"
#include "../Camera/Camera.h"

class CameraController : public AComponent
{
public:
	CameraController(AGameObject* freeCam, AGameObject* pawn);
	~CameraController();

	void Clone(AComponent* copy) override;
	void Perform() override;

	void AddCamera(Camera* cam);


private:
	std::vector<Camera*> camerasList;
	Camera* activeCam;

	bool isInFreeCam;
	AGameObject* freeCam;
	AGameObject* pawn;
};