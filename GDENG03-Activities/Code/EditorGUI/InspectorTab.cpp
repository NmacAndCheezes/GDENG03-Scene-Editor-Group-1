#include "InspectorTab.h"
#include "HierarchyTab.h"
#include "GameEngine/Managers/GameObjectManager.h"
#include "WindowSystem/Keyboard.h"

#include "GameEngine/Components/Renderer/MeshRenderer.h"
#include "GameEngine/Components/Physics/Rigidbody3D.h"
#include "GameEngine/Managers/PhysicsEngine.h"
#include <EditorStates/EditorBackend.h>
#include <EditorStates/EditorActions/EditorActionHistory.h>

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
    ImGui::Begin(name.c_str(), &isEnabled, flags); 

	if (hierarchy->GetSelectedObj() == nullptr || EditorBackend::get()->getState() != EditorBackend::EDIT)
	{
		ImGui::Text("Cannot edit values right now");
	}
	else
	{
		AGameObject* selected = hierarchy->GetSelectedObj();
		RenderGameObjectDetails(selected);
		
		bool toggles[] = { false, false };
		RenderAddComponentButton(toggles);
		HandleAddingNewComponents(selected, toggles);

		ImGui::Dummy(ImVec2(0.0f, 20.f));
	}

	ImGui::End();
}

void InspectorTab::RenderGameObjectDetails(AGameObject* selected)
{
	// declare temp vars
	bool objEnabled = selected->Enabled; 
	char objName[128]; 
	strcpy_s(objName, selected->Name.c_str()); 

	// render UI and receive user inputs
	ImGui::Checkbox("##ObjEnabled", &objEnabled); ImGui::SameLine(); 
	ImGui::InputText("##ObjName", objName, IM_ARRAYSIZE(objName)); 

	// apply user inputs on the obj
	selected->Enabled = objEnabled;
	if (Keyboard::IsKeyPressed(VK_RETURN) && std::string(objName) != "") selected->Name = objName;

	// render each components
	for (auto& component : selected->GetAllComponents())
	{
		// create the dropdown menu
		ImGui::AlignTextToFramePadding();
		bool isDropdownOpen = ImGui::TreeNodeEx(("##" + component->GetName()).c_str(), ImGuiTreeNodeFlags_AllowItemOverlap);

		// render the enable checkbox (same line as the dropdown)
		bool compEnabled = component->Enabled;
		ImGui::SameLine();
		ImGui::Checkbox(("##" + component->GetName() + "Enabled").c_str(), &compEnabled);
		component->Enabled = compEnabled;

		// render the component name (same line as the dropdown)
		ImGui::SameLine();
		ImGui::Text((component->GetName() + " Component").c_str());

		// render the delete button (same line as the dropdown but right-aligned)
		bool willDelete = false; 
		if (component->GetType() != EComponentTypes::Transform)
		{
			std::string compPopup = "ComponentPopup##" + component->GetName();

			// set position of button then check for user inputs
			ImGui::SameLine(ImGui::GetContentRegionMax().x + ImGui::GetScrollX() - ImGui::CalcTextSize("[?]").x);
			if (ImGui::Button(("[?]##" + component->GetName()).c_str())) 
			{
				ImGui::OpenPopup(compPopup.c_str()); 
			}

			// create the popup 
			if (ImGui::BeginPopup(compPopup.c_str())) 
			{
				ImGui::MenuItem("Delete Component", "", &willDelete); 
				ImGui::EndPopup(); 
			}

			// check another user input to confirm deletion of component
			if (willDelete) 
			{
				selected->DetachComponent(component); 
			}
		}

		// if dropdown is open, render the UI of the component 
		if (isDropdownOpen)
		{
			if (!willDelete) component->RenderUI();
			ImGui::TreePop();
		}
			
		ImGui::Dummy(ImVec2(0.0f, 10.f));
	}
}

void InspectorTab::RenderAddComponentButton(bool* toggles)
{
	float midPoint = ImGui::GetContentRegionAvail().x / 2.f;
	float offset = ImGui::CalcTextSize("Add Component").x / 2.f;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + midPoint - offset);

	std::string addCompPopup = "AddComponentPopup";
	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup(addCompPopup.c_str()); 
	}

	if (ImGui::BeginPopup(addCompPopup.c_str())) 
	{
		ImGui::SeparatorText("Components");

		ImGui::MenuItem("Mesh Renderer", "", &toggles[0]);
		ImGui::MenuItem("Rigidbody3D", "", &toggles[1]);

		ImGui::EndPopup();
	}
}

void InspectorTab::HandleAddingNewComponents(AGameObject* selected, bool* toggles)
{
	if (toggles[0])
	{
		auto rendererList = selected->GetComponentsOfType(EComponentTypes::Renderer);
		bool hasRenderer = false;
		for (auto physicsComp : rendererList)
		{
			if (dynamic_cast<MeshRenderer*>(physicsComp) != nullptr) hasRenderer = true;
		}

		if (!hasRenderer)
		{
			MeshRenderer* renderer = new MeshRenderer();
			renderer->LoadPrimitive(EPrimitiveMeshTypes::Cube, true);
			selected->AttachComponent(renderer);
			GameObjectManager::GetInstance()->BindRendererToShader(renderer);
		}
	}
	else if (toggles[1])
	{
		auto physicsList = selected->GetComponentsOfType(EComponentTypes::Physics);
		bool hasRB = false;
		for (auto physicsComp : physicsList)
		{
			if (dynamic_cast<RigidBody3D*>(physicsComp) != nullptr) hasRB = true;
		}

		if (!hasRB)
		{
			RigidBody3D* rb = new RigidBody3D(EPrimitiveMeshTypes::Cube);
			selected->AttachComponent(rb);
			PhysicsEngine::GetInstance()->RegisterRigidBody(rb);
			rb->UpdateTransform();
			rb->Mass = 10.f;
		}
	}
}