#pragma once
#include"AUITab.h"


class DockSpace : public AUITab
{
public:
	DockSpace();
	~DockSpace();

	void RenderUI() override;

protected:
	void InitializeImGuiFlags() override;


private:
	ImGuiDockNodeFlags dockSpaceFlags;
};