#pragma once
#include "AUITab.h"
#include "GameEngine/GameObjects/AGameObject.h"

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
	void RenderGameObjectDetails(AGameObject* selected);
	void RenderAddComponentButton(bool* toggles);
	void HandleAddingNewComponents(AGameObject* selected, bool* toggles);


private:
	HierarchyTab* hierarchy;
};

