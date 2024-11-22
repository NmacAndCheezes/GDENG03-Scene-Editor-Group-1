#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h> 
#include <imgui_impl_win32.h> 


class IRenderableUI
{
public:
	virtual void RenderUI() = 0;
};