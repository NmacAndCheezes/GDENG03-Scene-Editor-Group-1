#include "HierarchyTab.h"
#include "GameEngine/Managers/GameObjectManager.h"
#include "../WindowSystem/Keyboard.h"

HierarchyTab::HierarchyTab() : AUITab(EditorGUIManager::TabNames::HIERARCHY_TAB.data()), selectedObject(nullptr)
{
    InitializeImGuiFlags();
}

HierarchyTab::~HierarchyTab() 
{ 
	
}

void HierarchyTab::InitializeImGuiFlags() 
{
    flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar;
}

void HierarchyTab::RenderUI()
{
	ImGui::Begin(name.c_str(), &isEnabled, flags);

    // child windows are needed to receive drag&drop inputs
    ImGui::BeginChild("##DroppableHierarchy");  
	for (auto obj : GameObjectManager::GetInstance()->GetAllGameObjects())  // render all root objs
	{
		if (!obj->IsEditorObject()) PrintObject(obj);
	}
    ImGui::EndChild(); 

    // check for dropped inputs
    if (ImGui::BeginDragDropTarget()) 
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_OBJ_DRAG")) 
        {
            IM_ASSERT(payload->DataSize == sizeof(AGameObject*)); 

            AGameObject* droppedObj = *(AGameObject**)payload->Data; 
            if (droppedObj->GetParent() != nullptr)
            {
                GameObjectManager::GetInstance()->AddRootObject(droppedObj); 
            }
        }
        ImGui::EndDragDropTarget(); 
    }

	ImGui::End(); 

    if (Keyboard::IsKeyPressed(VK_DELETE) && selectedObject != nullptr)
    {
        GameObjectManager::GetInstance()->DeleteObject(selectedObject);
        selectedObject = nullptr;
    }
}

AGameObject* HierarchyTab::GetSelectedObj()
{
    return selectedObject;
}

void HierarchyTab::PrintObject(AGameObject* obj)
{
    std::string objID_str = std::to_string(obj->GetInstanceID()); 
    bool hasChildren = obj->GetChildList().size() > 0; 
    bool isDropdownOpen = false;

    // create a dropdown if obj has child(ren)
    if (hasChildren)
    {
        ImGui::AlignTextToFramePadding();  
        ImGui::PushID(obj->GetInstanceID());  
        isDropdownOpen = ImGui::TreeNodeEx("", ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::SameLine();  
    }
    // else if no child, then align UI with the rest of the objs
    else
    {
        ImGui::Indent(30);
    }
    

    // make the name of the obj clickable/selectable
    if (ImGui::Button((obj->Name + "##" + objID_str).c_str()))  
    {
        selectedObject = obj;  
    }


    // check for dragging inputs 
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) 
    {
        // Set payload to carry the gameobject itself
        ImGui::SetDragDropPayload("HIERARCHY_OBJ_DRAG", &obj, sizeof(AGameObject*)); 
        ImGui::EndDragDropSource();
    }

    // check for dropped inputs
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_OBJ_DRAG"))
        {
            IM_ASSERT(payload->DataSize == sizeof(AGameObject*));

            AGameObject* droppedObj = *(AGameObject**)payload->Data;
            if (obj->GetParent() == nullptr) GameObjectManager::GetInstance()->RemoveObject(droppedObj);
            obj->AttachChild(droppedObj);
        }
        ImGui::EndDragDropTarget();
    }
    

    // render the children if they exist
    if (hasChildren)
    {
        if (isDropdownOpen)  
        {
            for (auto child : obj->GetChildList()) 
            {
                PrintObject(child); 
            }

            ImGui::TreePop(); 
        }
        ImGui::PopID(); 
    }
    // else just revert the indentation to prepare the next obj
    else
    {
        ImGui::Unindent(30); 
    }
}