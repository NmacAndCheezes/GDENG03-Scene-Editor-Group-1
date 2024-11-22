#include "DebugLogUI.h"
#include "GameEngine/Debug.h"


DebugLogUI::DebugLogUI() : AUITab(EditorGUIManager::TabNames::CONSOLE_TAB.data())
{
	InitializeImGuiFlags();
}

DebugLogUI::~DebugLogUI()
{

}

void DebugLogUI::InitializeImGuiFlags()
{
	flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar;
}

void DebugLogUI::RenderUI()
{
	ImGui::Begin(name.c_str(), &isEnabled, flags);

	if(ImGui::Button("Clear"))
	{
		Debug::ClearLog();
	}

	for (auto i : Debug::get()->m_logs)
	{
		ImGui::Text(i.c_str());
		ImGui::Separator();
	}

	ImGui::End();
}