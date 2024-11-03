#pragma once
#include "imgui-master/imgui.h"
#include "imgui-master/imgui_impl_dx11.h"
#include "imgui-master/imgui_impl_win32.h"

class AUIScreen
{
public:
	AUIScreen();
	virtual void update() = 0;
	~AUIScreen();

	void setIsActive(bool isActive);
	bool getIsActive();
protected:
	bool isActive = true;
};

