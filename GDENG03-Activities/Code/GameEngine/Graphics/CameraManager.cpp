#include "CameraManager.h"


CameraManager* CameraManager::sharedInstance = nullptr; 

CameraManager* CameraManager::GetInstance()
{
    if (sharedInstance == nullptr)
    {
        sharedInstance = new CameraManager();
    }

    return sharedInstance; 
}

void CameraManager::AddCamera(Camera* newCamera)
{
    if (newCamera == nullptr) return;

    auto itr = std::find(camerasList.begin(), camerasList.end(), newCamera);
    if (itr != camerasList.end()) return;

    camerasList.push_back(newCamera);
}

void CameraManager::RemoveCamera(Camera* oldCamera)
{
    if (oldCamera == nullptr) return;

    auto itr = std::find(camerasList.begin(), camerasList.end(), oldCamera);
    if (itr == camerasList.end()) return;

    camerasList.erase(itr);
    camerasList.shrink_to_fit();
}

std::vector<Camera*> CameraManager::GetCamerasList()
{
    return camerasList;
}