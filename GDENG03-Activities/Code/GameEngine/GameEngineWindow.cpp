#include "GameEngineWindow.h"
#include "Debug.h"
#include "EngineTime.h"
#include "EditorGUI/EditorGUIManager.h"
#include "Managers/GameObjectManager.h"
#include "Managers/PhysicsEngine.h"
#include "WindowSystem/FileDialogs.h"
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
#include <EditorStates/EditorActions/EditorActionHistory.h>

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
	EditorActionHistory::initialize();
	EditorBackend::initialize();
	Debug::initialize();

	GraphicsEngine::GetInstance()->Init();
	GraphicsEngine::GetInstance()->SetViewport(width, height); 
	swapChain = GraphicsEngine::GetInstance()->CreateSwapChain(hWnd, width, height); 

	EngineTime::Initialize();
	EditorGUIManager::GetInstance()->Initialize(hWnd); 
	PhysicsEngine::GetInstance()->Init();

	FileDialogs::SetDeafultHWND(hWnd);


	// setup the objects
	FreeCameraObject* freeCam = new FreeCameraObject(width, height);  
	freeCam->GetTransform()->Position = { 0.0f, 30.0f, -30.0f };
	freeCam->GetTransform()->Rotate(50.0f, 0.0f, 0.0f);
	GameObjectManager::GetInstance()->AddRootObject(freeCam); 

#if 0
	ModelObject* bunny = new ModelObject("bunny.obj");
	bunny->GetTransform()->Position = Vector3(10.0f, 0.0f, 0.0f);
	bunny->GetTransform()->LocalScale = Vector3(10.0f, 10.0f, 10.0f);
	GameObjectManager::GetInstance()->AddRootObject(bunny);

	ModelObject* armadillo = new ModelObject("armadillo.obj");
	armadillo->GetTransform()->Position = Vector3(-10.0f, 0.0f, 0.0f);
	//armadillo->GetTransform()->LocalScale = Vector3(10.0f, 10.0f, 10.0f);
	GameObjectManager::GetInstance()->AddRootObject(armadillo);

	ModelObject* Lucy = new ModelObject("statue.obj");
	Lucy->GetTransform()->Position = Vector3(0.0f, 0.0f, 0.0f);
	Lucy->GetTransform()->LocalScale = Vector3(10.0f, 10.0f, 10.0f);
	GameObjectManager::GetInstance()->AddRootObject(Lucy);
#endif
#if 0
	for (int i = 0; i < 5; i++)
	{
		PrimitiveObject* cube = new PrimitiveObject("cube", EPrimitiveMeshTypes::Cube, true);
		cube->GetTransform()->Position = Vector3(
			MathUtils::RandFloatWithRange(-5.f, 5.f),
			10.f,
			MathUtils::RandFloatWithRange(-5.f, 5.f)
			);

		GameObjectManager::GetInstance()->AddRootObject(cube);
	}

	ModelObject* model = new ModelObject("teapot3.obj", "brick.png");
	model->GetTransform()->ScaleUniformly(0.35f);
	GameObjectManager::GetInstance()->AddRootObject(model);
#endif
}

void GameEngineWindow::OnUpdate()
{
	swapChain->ClearBuffer(0.0f, 0.0f, 0.0f);

	accumulator += (float)EngineTime::GetDeltaTime();
	float secsPerFrame = 1.f / (float)fps;
	while (accumulator >= secsPerFrame)
	{
		accumulator -= secsPerFrame;
		GameObjectManager::GetInstance()->UpdateEditor(secsPerFrame);

		if (EditorBackend::get()->getState() == EditorBackend::PLAY)
		{
			GameObjectManager::GetInstance()->UpdateGame(secsPerFrame);
			PhysicsEngine::GetInstance()->UpdateWorld(secsPerFrame);
			float factor = accumulator / secsPerFrame;
			PhysicsEngine::GetInstance()->UpdateRigidBodies(factor);
		}
		else if (EditorBackend::get()->getState() == EditorBackend::PAUSE)
		{
			if (EditorBackend::get()->getIsFrameStep())
			{
				GameObjectManager::GetInstance()->UpdateGame(secsPerFrame);
				PhysicsEngine::GetInstance()->UpdateWorld(secsPerFrame);
				float factor = accumulator / secsPerFrame;
				PhysicsEngine::GetInstance()->UpdateRigidBodies(factor);
				EditorBackend::get()->endFrameStep();
			}
		}
		else if (EditorBackend::get()->getState() == EditorBackend::EDIT)
		{
			int CTRL_Y = 89;
			int CTRL_Z = 90;
			int KEY_DELETE = 46;

			if (Keyboard::IsKeyPressed(CTRL_Z)) {
				EditorActionHistory::get()->Undo();
			}
			else if (Keyboard::IsKeyPressed(CTRL_Y)) {
				EditorActionHistory::get()->Redo();
			}
		}

		Keyboard::FlushCharBuffer();
	}
	

	GameObjectManager::GetInstance()->Draw(); 
	EditorGUIManager::GetInstance()->Render();
	swapChain->Present(true); 

	Keyboard::FlushEventsBuffer(); 
	Mouse::FlushEventsBuffer(); 
}

void GameEngineWindow::OnDestroy() 
{ 
	Window::OnDestroy();

	GameObjectManager::Destroy();
	EditorGUIManager::Destroy(); 
	GraphicsEngine::GetInstance()->Release();
	PhysicsEngine::GetInstance()->Release();
	Debug::destroy();
	EditorBackend::destroy();
	EditorActionHistory::destroy();
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
