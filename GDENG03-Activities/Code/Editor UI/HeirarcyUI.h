#pragma once
#include "AUIScreen.h"
#include <vector>
#include "../GameEngine/GameObjects/AGameObject.h"

class HeirarcyUI : public AUIScreen
{
public:
	HeirarcyUI();
	virtual void update() override;
	void printChild(AGameObject* obj, int layer);
	~HeirarcyUI();
private:
	AGameObject* selectedObject = nullptr;
private:
	friend class InspectorUI;
};

