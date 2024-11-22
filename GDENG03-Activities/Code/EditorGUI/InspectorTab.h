#pragma once
#include "AUITab.h"

class HierarchyTab;

class InspectorTab : public AUITab
{
public:
	InspectorTab(HierarchyTab* hierarchy);
	~InspectorTab();

	void RenderUI() override;

protected:
	void InitializeImGuiFlags() override;


private:
	HierarchyTab* hierarchy;

	bool enabled;
	char objName[128];
	float position[3];
	float eulerAngle[3];
	float scale[3];
};

