#pragma once
#include "AUITab.h"

class MenuTab : public AUITab
{
public:
	MenuTab();
	~MenuTab();

	void RenderUI() override;


private:
	void RenderFilesMenu();
	void RenderTabsMenu();
	void RenderToolsMenu();
	void RenderAboutMenu();
	void RenderSceneStatesMenu();
};