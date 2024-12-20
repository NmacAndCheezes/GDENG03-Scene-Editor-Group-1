#include "CameraManager.h"
#include "../GameObjects/AGameObject.h"


CameraManager* CameraManager::sharedInstance = nullptr; 

CameraManager* CameraManager::GetInstance()
{
    if (sharedInstance == nullptr)
    {
        sharedInstance = new CameraManager();
    }

    return sharedInstance; 
}

void CameraManager::Destroy()
{
    if (!sharedInstance) return;

    /*for (auto camera : sharedInstance->camerasList) 
    {
        if (camera) delete camera->GetOwner();
    }*/

    delete sharedInstance;
}

//void CameraManager::DeleteCameraScene()
//{
//    for (int i = (int)sharedInstance->camerasList.size() - 1; i >= 0; i--)
//    {
//        Camera* cam = sharedInstance->camerasList[i]; 
//        if (!cam->GetOwner()->IsEditorObject())
//        {
//
//        }
//    }
//}

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