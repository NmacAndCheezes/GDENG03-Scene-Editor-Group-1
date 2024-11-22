#pragma once
#include <vector>
#include "AUITab.h"
#include "GameEngine/GameObjects/AGameObject.h"


class HierarchyTab : public AUITab
{
public:
	HierarchyTab();
	~HierarchyTab(); 

	void RenderUI() override;
	AGameObject* GetSelectedObj();

protected:
	void InitializeImGuiFlags() override;

private:
	void PrintObject(AGameObject* obj);


private:
	AGameObject* selectedObject;
};