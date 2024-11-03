#pragma once
#include <vector>
#include "AUIScreen.h"
#include "../GameEngine/GameObjects/AGameObject.h"


class HierarchyUI : public AUIScreen
{
public:
	HierarchyUI();
	~HierarchyUI(); 

	void update() override;

private:
	void PrintObject(AGameObject* obj);


private:
	AGameObject* selectedObject;
private:
	friend class InspectorUI;
};