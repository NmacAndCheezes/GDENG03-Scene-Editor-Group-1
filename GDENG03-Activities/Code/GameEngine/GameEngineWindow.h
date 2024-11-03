#pragma once
#include "../WindowSystem/Window.h"
#include "../DirectXClasses/SwapChain.h"
#include "../GameEngine/Components/Transform.h"


class GameEngineWindow : public Window
{
public:
	GameEngineWindow(int fps);
	~GameEngineWindow();

protected:
	void OnCreate(HWND hWnd) override;
	void OnUpdate() override;
	void OnDestroy() override;
	LRESULT CALLBACK HandleWindowMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;


private:
	SwapChain* swapChain;
	int fps;
	float currDelta;

	Transform* t1;
	Transform* t2;
	Transform* t3;
	Transform* t4;
	Transform* t5;
};