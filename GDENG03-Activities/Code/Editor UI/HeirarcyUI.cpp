#include "HeirarcyUI.h"
#include "../GameEngine/Managers/GameObjectManager.h"
#include <algorithm>
#include <iostream>

HeirarcyUI::HeirarcyUI()
{
}

void HeirarcyUI::update()
{
	ImGui::Begin("Heirarcy");
	
	for (auto i : GameObjectManager::GetInstance()->GetAllObjects())
	{
		if (i->GetParent() != nullptr) continue;

		if (ImGui::Button(i->Name.c_str()))
		{
			selectedObject = i;
		}
		printChild(i, 0);
	}

	ImGui::End();
}

void HeirarcyUI::printChild(AGameObject* obj, int layer)
{
	for (auto i : obj->GetChildList())
	{
		ImGui::Indent(10 + (10 * layer));
		if (ImGui::Button(i->Name.c_str()))
		{
			selectedObject = i;
		}
		
		ImGui::Unindent(10 + (10 * layer));
		if (i->GetChildList().size() != 0) printChild(i, layer + 1);
		
	}
}

HeirarcyUI::~HeirarcyUI()
{
}
