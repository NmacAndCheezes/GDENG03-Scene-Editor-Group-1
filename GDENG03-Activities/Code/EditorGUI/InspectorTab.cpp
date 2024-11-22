#include "InspectorTab.h"
#include "HierarchyTab.h"
#include "GameEngine/Managers/GameObjectManager.h"
#include "WindowSystem/Keyboard.h"

#include "GameEngine/Components/Physics/Rigidbody3D.h"
#include "GameEngine/Managers/PhysicsEngine.h"

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
	// refactor next time
	auto physicsList = selected->GetComponentsOfType(EComponentTypes::Physics);
	bool hasRB = false;
	for (auto physicsComp : physicsList)
	{
		if (dynamic_cast<RigidBody3D*>(physicsComp) != nullptr) hasRB = true;
	}

	if (!hasRB)
	{
		if (ImGui::Button("Add RigidBody3D"))
		{
			RigidBody3D* rb = new RigidBody3D(EPrimitiveMeshTypes::Cube);
			selected->AttachComponent(rb);
			PhysicsEngine::GetInstance()->RegisterRigidBody(rb);
			rb->UpdateTransform();
			rb->Mass = 10.f;
		}
	}
	else
	{
		if (ImGui::Button("Remove RigidBody3D"))
		{
			auto physicsList = selected->GetComponentsOfType(EComponentTypes::Physics);
			bool hasRB = false;
			RigidBody3D* rb = nullptr;
			for (auto physicsComp : physicsList)
			{
				if (dynamic_cast<RigidBody3D*>(physicsComp) != nullptr) rb = (RigidBody3D*)physicsComp;
			}

			PhysicsEngine::GetInstance()->UnregisterRigidBody(rb);
			selected->DetachComponent(rb);
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 20.f));

	ImGui::End();
}