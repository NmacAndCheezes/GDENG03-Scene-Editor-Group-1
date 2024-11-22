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
	Transform* t = selected->GetTransform();

	enabled = selected->Enabled;
	strcpy_s(objName, selected->Name.c_str());

	position[0] = t->LocalPosition.x;
	position[1] = t->LocalPosition.y;
	position[2] = t->LocalPosition.z;

	eulerAngle[0] = t->GetLocalEulerAngles().x;
	eulerAngle[1] = t->GetLocalEulerAngles().y;
	eulerAngle[2] = t->GetLocalEulerAngles().z;

	scale[0] = t->LocalScale.x;
	scale[1] = t->LocalScale.y;
	scale[2] = t->LocalScale.z;

	ImGui::Checkbox("##ObjEnabled", &enabled); ImGui::SameLine();
	ImGui::InputText("##ObjName", objName, IM_ARRAYSIZE(objName));

	ImGui::AlignTextToFramePadding();
	if (ImGui::TreeNodeEx("Transform Component", ImGuiTreeNodeFlags_AllowItemOverlap))
	{
		ImGui::DragFloat3("Position", position); 
		ImGui::DragFloat3("Rotation", eulerAngle); 
		ImGui::DragFloat3("Scale", scale);
		ImGui::TreePop(); 
	}

	Vector3 diffEuler = Vector3(eulerAngle) - t->GetLocalEulerAngles();

	selected->Enabled = enabled;
	if (Keyboard::IsKeyPressed(VK_RETURN) && std::string(objName) != "") selected->Name = objName; // reupdate gameobjectmanaer

	if (Vector3(position) != t->LocalPosition)           t->SetLocalPosition(Vector3(position));
	if (Vector3(eulerAngle) != t->GetLocalEulerAngles()) t->Rotate(diffEuler);
	if (Vector3(scale) != t->LocalScale)                 t->SetLocalScale(Vector3(scale));

	ImGui::End();
}