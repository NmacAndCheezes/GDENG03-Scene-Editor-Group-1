#include "InspectorTab.h"
#include "HierarchyTab.h"
#include "../GameEngine/Managers/GameObjectManager.h"
#include "../WindowSystem/Keyboard.h"


InspectorTab::InspectorTab(HierarchyTab* hierarchy) : AUITab(EditorGUIManager::TabNames::INSPECTOR_TAB.data()), hierarchy(hierarchy)
{
    InitializeImGuiFlags();  
}

InspectorTab::~InspectorTab()
{

}

void InspectorTab::InitializeImGuiFlags()
{
    flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar; 
}

void InspectorTab::RenderUI()
{
	if (hierarchy->GetSelectedObj() == nullptr) return;

    ImGui::Begin(name.c_str(), &isEnabled, flags); 

	AGameObject* selected = hierarchy->GetSelectedObj();
	bool objEnabled = selected->Enabled;
	char objName[128];
	strcpy_s(objName, selected->Name.c_str());

	ImGui::Checkbox("##ObjEnabled", &objEnabled); ImGui::SameLine(); 
	ImGui::InputText("##ObjName", objName, IM_ARRAYSIZE(objName)); 

	selected->Enabled = objEnabled; 
	if (Keyboard::IsKeyPressed(VK_RETURN) && std::string(objName) != "") selected->Name = objName;

	for (auto& component : selected->GetAllComponents())
	{
		bool compEnabled = component->Enabled;

		ImGui::AlignTextToFramePadding();
		ImGui::SetNextItemOpen(true);
		if (ImGui::TreeNodeEx(("##" + component->GetName()).c_str(), ImGuiTreeNodeFlags_AllowItemOverlap))
		{
			ImGui::SameLine();
			ImGui::Checkbox(("##" + component->GetName() + "Enabled").c_str(), &compEnabled); 
			ImGui::SameLine();
			ImGui::Text((component->GetName() + " Component").c_str());
			component->RenderUI();
			ImGui::TreePop(); 
		}
		ImGui::Dummy(ImVec2(0.0f, 10.f));

		component->Enabled = compEnabled; 
	}
	ImGui::Dummy(ImVec2(0.0f, 20.f));

	ImGui::End();
}