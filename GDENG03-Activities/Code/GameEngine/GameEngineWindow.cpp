#include "GameEngineWindow.h"
#include "EngineTime.h"
#include "Managers/GameObjectManager.h"
#include "GameObjects/FreeCameraObject.h"

#include "GameObjects/Primitives/CubeObject.h"
#include "GameObjects/Primitives/ConeObject.h"
#include "GameObjects/Primitives/CylinderObject.h"
#include "GameObjects/Primitives/SphereObject.h"
#include "../Editor UI/EditorUIManager.h"


GameEngineWindow::GameEngineWindow(int fps) : fps(fps), currDelta(0.f)
{
	windowClassName = "GameEngineClass";
}

GameEngineWindow::~GameEngineWindow()
{ 

}

void GameEngineWindow::OnCreate(HWND hWnd)
{
	// initialize game engine
	GraphicsEngine::GetInstance()->Init();
	GraphicsEngine::GetInstance()->SetViewport(width, height);
	swapChain = GraphicsEngine::GetInstance()->CreateSwapChain(hWnd, width, height);
	EngineTime::Initialize();
	EditorUIManager::get()->initialize(hWnd);

	// setup the objects
	FreeCameraObject* freeCam = new FreeCameraObject(width, height); 
	freeCam->GetTransform()->Position = { 0.0f, 10.0f, 0.0f }; 
	GameObjectManager::GetInstance()->AddObject(freeCam); 


	std::vector<AGameObject*> objsList;
	int rowSize = 7; int colSize = 7;
	float rowSpacing = 5.f; float colSpacing = 5.f;
	int sphereNum = 0, cylinderNum = 0, coneNum = 0, cubeNum = 0;  

	for (int i = 0; i < colSize; i++)   
	{
		for (int j = 0; j < rowSize; j++)  
		{
			int randNum = rand() % 4; 
			AGameObject* randObj = nullptr; 

			switch (randNum)  
			{
				case 0:
					{ randObj = new SphereObject("Sphere" + std::to_string(sphereNum)); sphereNum++; break; }
				case 1: 
					{ randObj = new CylinderObject("Cylinder" + std::to_string(cylinderNum)); cylinderNum++; break; } 
				case 2:
					{ randObj = new ConeObject("Cone" + std::to_string(coneNum)); coneNum++; break; }
				case 3:
				default:
					{ randObj = new CubeObject("Cube" + std::to_string(cubeNum)); cubeNum++; break; }
			}

			float x = j * rowSpacing - (rowSize / 2.f - 0.5f) * rowSpacing; 
			float z = i * colSpacing - (colSize / 2.f - 0.5f) * colSpacing;  
			randObj->GetTransform()->Position = { x , 0, z };  

			randNum = (i == 0) ? colSize * 4 : rand() % colSize * 4 + 1;  
			int parentIndex = (randNum / 4) + (i - 1) * rowSize;
			if (randNum == colSize * 4) GameObjectManager::GetInstance()->AddObject(randObj);
			else objsList[parentIndex]->AttachChild(randObj); 

			objsList.push_back(randObj);
		}
	}
}

void GameEngineWindow::OnUpdate()
{
	swapChain->ClearBuffer(0.4f, 0.4f, 0.6f);

	currDelta += (float)EngineTime::GetDeltaTime();
	float secsPerFrame = 1.f / (float)fps;
	if (currDelta >= secsPerFrame) 
	{
		currDelta -= secsPerFrame; 
		GameObjectManager::GetInstance()->Update(secsPerFrame); 

		Keyboard::FlushEventsBuffer();  
		Keyboard::FlushCharBuffer(); 
		Mouse::FlushEventsBuffer(); 
	}

	GameObjectManager::GetInstance()->Draw();
	EditorUIManager::get()->update();
	swapChain->Present(true); 
}

void GameEngineWindow::OnDestroy() 
{ 
	Window::OnDestroy();

	GraphicsEngine::GetInstance()->Release();
	EditorUIManager::get()->destroy();
	// engine time???
	// other more managers???
}

LRESULT GameEngineWindow::HandleWindowMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return Window::HandleWindowMessages(hWnd, msg, wParam, lParam);
}