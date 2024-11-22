#include "MenuTab.h"

MenuTab::MenuTab() : AUITab(EditorGUIManager::TabNames::MENU_TAB.data()) 
{

}

MenuTab::~MenuTab()
{

}

void MenuTab::RenderUI()
{
	if (ImGui::BeginMainMenuBar())
	{
        RenderFilesMenu();
        RenderTabsMenu();
        RenderToolsMenu();
        RenderAboutMenu();

		ImGui::EndMainMenuBar();
	}
}

void MenuTab::RenderFilesMenu()
{
    if (ImGui::BeginMenu("Files"))
    {
        if (ImGui::MenuItem("New"))
        {

        }

        if (ImGui::MenuItem("Open", "Ctrl+O"))
        {

        }

        if (ImGui::MenuItem("Save", "Ctrl+S"))
        {

        }

        if (ImGui::MenuItem("Save As.."))
        {

        }

        ImGui::EndMenu();
    }
}

void MenuTab::RenderTabsMenu()
{
    if (ImGui::BeginMenu("Tabs"))
    {
        if (ImGui::MenuItem("Hierarchy"))
        {
            AUITab* hierarchy = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::HIERARCHY_TAB.data());
            if (hierarchy) hierarchy->Enabled = true;
        }

        if (ImGui::MenuItem("Inspector"))
        {
            AUITab* inspector = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::INSPECTOR_TAB.data());
            if (inspector) inspector->Enabled = true;
        }

        if (ImGui::MenuItem("Console"))
        {
            AUITab* console = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::CONSOLE_TAB.data());
            if (console) console->Enabled = true;
        }


        ImGui::EndMenu();
    }
}

void MenuTab::RenderToolsMenu()
{
    if (ImGui::BeginMenu("Tools"))
    {
        if (ImGui::MenuItem("Color Picker"))
        {
            AUITab* colorPicker = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::COLOR_PICKER_TAB.data());
            if (colorPicker) colorPicker->Enabled = true;
        }

        ImGui::EndMenu(); 
    }
}

void MenuTab::RenderAboutMenu()
{
    if (ImGui::BeginMenu("About")) 
    {
        if (ImGui::MenuItem("Credits")) 
        {
            AUITab* about = EditorGUIManager::GetInstance()->GetTab(EditorGUIManager::TabNames::CREDITS_TAB.data()); 
            if (about) about->Enabled = true; 
        }

        ImGui::EndMenu(); 
    }
}