#pragma once
#include "AUIScreen.h"

class HeirarcyUI;
class HierarchyUI;

class InspectorUI : public AUIScreen
{
public:
	InspectorUI(HeirarcyUI* HeirarcyUI);
	InspectorUI(HierarchyUI* HierarchyUI);
	virtual void update() override;
	~InspectorUI();

private:
	HeirarcyUI* heirarcyUI;
	HierarchyUI* hierarchyUI;
	float pos[3];
	float rot[3];
	float scale[3];
};

