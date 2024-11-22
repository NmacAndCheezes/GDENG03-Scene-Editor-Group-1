#include "GameEngineWindow.h"
#include "Debug.h"
#include "EngineTime.h"
#include "EditorGUI/EditorGUIManager.h"
#include "Managers/GameObjectManager.h"
#include "Managers/PhysicsEngine.h"
#include "GameObjects/FreeCameraObject.h"

#include "GameObjects/Primitives/CubeObject.h"
#include "GameObjects/Primitives/ConeObject.h"
#include "GameObjects/Primitives/CylinderObject.h"
#include "GameObjects/Primitives/SphereObject.h"
#include "GameObjects/Primitives/PlaneObject.h"
#include "GameObjects/ModelObject.h"
#include "GameObjects/PhysicsObject.h"
#include "Managers/PhysicsEngine.h"
#include "MathUtils.h"

#include "EditorStates/EditorBackend.h"

GameEngineWindow::GameEngineWindow(int fps) : fps(fps), accumulator(0.f)
{
	windowClassName = "GameEngineClass";
}

GameEngineWindow::~GameEngineWindow()
{ 

}

void GameEngineWindow::OnCreate(HWND hWnd)
{
	// initialize game engine
	EditorBackend::initialize();
	Debug::initialize();
	GraphicsEngine::GetInstance()->Init();
	GraphicsEngine::GetInstance()->SetViewport(width, height); 
	swapChain = GraphicsEngine::GetInstance()->CreateSwapChain(hWnd, width, height); 
	EngineTime::Initialize();
	EditorGUIManager::GetInstance()->Initialize(hWnd); 
	PhysicsEngine::GetInstance()->Init();


	// setup the objects
	FreeCameraObject* freeCam = new FreeCameraObject(width, height);  
	freeCam->GetTransform()->Position = { 0.0f, -60.0f, -70.0f };
	freeCam->GetTransform()->Rotate(50.0f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddObject(freeCam); 


	PhysicsObject* phy1 = new PhysicsObject(EPrimitiveMeshTypes::Plane);
	phy1->GetTransform()->Position = Vector3(0.0f, -160.0f, 0.0f);
	phy1->GetTransform()->LocalScale = Vector3(7.0f, 1.0f, 7.0f);
	GameObjectManager::GetInstance()->AddObject(phy1); 
	phy1->GetRB()->BodyType = rp3d::BodyType::STATIC;

	for (int i = 0; i < 10; i++)
	{ 
		PhysicsObject* phy = new PhysicsObject(EPrimitiveMeshTypes::Cube); 
		phy->GetTransform()->Position = Vector3(MathUtils::RandFloatWithRange(-3.f, 3.f), 
			MathUtils::RandFloatWithRange(-15.f, 15.f), 
			MathUtils::RandFloatWithRange(-3.f, 3.f));

		phy->GetTransform()->Rotate(MathUtils::RandFloatWithRange(15.f, 75.f), 
			MathUtils::RandFloatWithRange(15.f, 75.f),
			MathUtils::RandFloatWithRange(15.f, 75.f));

		/*phy->GetTransform()->LocalScale = Vector3(MathUtils::RandFloatWithRange(0.5f, 1.4f), 
			MathUtils::RandFloatWithRange(0.5f, 1.4f), 
			MathUtils::RandFloatWithRange(0.5f, 1.4f));*/

		GameObjectManager::GetInstance()->AddObject(phy); 
		phy->GetRB()->BodyType = rp3d::BodyType::STATIC;
	}



	/*std::vector<AGameObject*> objsList; 
	int rowSize = 15; int colSize = 15; 
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
				{ randObj = new SphereObject("Sphere" + std::to_string(sphereNum), true); sphereNum++; break; } 
				case 1:
				{ randObj = new CylinderObject("Cylinder" + std::to_string(cylinderNum), true); cylinderNum++; break; }
				case 2:
				{ randObj = new ConeObject("Cone" + std::to_string(coneNum), true); coneNum++; break; }
				case 3: default:
				{ randObj = new CubeObject("Cube" + std::to_string(cubeNum), true); cubeNum++; break; }
			}

			float x = j * rowSpacing - (rowSize / 2.f - 0.5f) * rowSpacing; 
			float z = i * colSpacing - (colSize / 2.f - 0.5f) * colSpacing; 
			randObj->GetTransform()->Position = { x , 0, z }; 

			randNum = (i == 0) ? rowSize * 4 : rand() % rowSize * 4 + 1;
			int parentIndex = (randNum / 4) + (i - 1) * rowSize; 
			if (randNum == rowSize * 4) GameObjectManager::GetInstance()->AddObject(randObj);
			else objsList[parentIndex]->AttachChild(randObj); 

			objsList.push_back(randObj); 
		}
	}*/
}

void GameEngineWindow::OnUpdate()
{
	swapChain->ClearBuffer(0.0f, 0.0f, 0.0f);
	//swapChain->ClearBuffer(0.4f, 0.4f, 0.6f);

	accumulator += (float)EngineTime::GetDeltaTime();
	float secsPerFrame = 1.f / (float)fps;
	while (accumulator >= secsPerFrame) 
	{
		accumulator -= secsPerFrame; 
		GameObjectManager::GetInstance()->Update(secsPerFrame);
		PhysicsEngine::GetInstance()->UpdateWorld(secsPerFrame);

		Keyboard::FlushCharBuffer();
	}

	float factor = accumulator / secsPerFrame;
	if(EditorBackend::get()->getState() == EditorBackend::PLAY)
		PhysicsEngine::GetInstance()->UpdateRigidBodies(factor);

	GameObjectManager::GetInstance()->Draw(); 
	EditorGUIManager::GetInstance()->Render();
	swapChain->Present(true); 

	Keyboard::FlushEventsBuffer(); 
	Mouse::FlushEventsBuffer(); 
}

void GameEngineWindow::OnDestroy() 
{ 
	Window::OnDestroy();

	PhysicsEngine::GetInstance()->Release();
	GameObjectManager::Destroy();
	EditorGUIManager::Destroy(); 
	GraphicsEngine::GetInstance()->Release();
	Debug::destroy();
	EditorBackend::destroy();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT GameEngineWindow::HandleWindowMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) 
	{ 
		return true; 
	}

	return Window::HandleWindowMessages(hWnd, msg, wParam, lParam);
}