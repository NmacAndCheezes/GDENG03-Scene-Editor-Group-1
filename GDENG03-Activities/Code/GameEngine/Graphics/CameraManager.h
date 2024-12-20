#pragma once
#include "../Components/Camera/Camera.h"


class CameraManager
{
public:
	static CameraManager* GetInstance();
	static void Destroy();
	CameraManager(const CameraManager&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;

	//static void DeleteCameraScene();

	void AddCamera(Camera* newCamera);
	void RemoveCamera(Camera* oldCamera);
	std::vector<Camera*> GetCamerasList();

private:
	CameraManager() {};
	~CameraManager() {};


private:
	static CameraManager* sharedInstance;

	std::vector<Camera*> camerasList;
};