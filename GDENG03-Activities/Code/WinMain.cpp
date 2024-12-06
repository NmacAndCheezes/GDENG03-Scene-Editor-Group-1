#include "GameEngine/GameEngineWindow.h"
#include <time.h>


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(NULL)); 

	GameEngineWindow gameEngine(120);  

	if (gameEngine.Init(960, 720, "Game Engine")) 
	{
		while (gameEngine.IsRunning())  
		{
			gameEngine.Update();  
		}
	}

	gameEngine.Release();

	return 0;
}