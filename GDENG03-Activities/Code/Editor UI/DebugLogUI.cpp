#include "DebugLogUI.h"
#include "../DebugLog/Debug.h"
DebugLogUI::DebugLogUI()
{
}

void DebugLogUI::update()
{
	ImGui::Begin("Debug");
	for (auto i : Debug::get()->m_logs)
	{
		ImGui::Text(i.c_str());
		ImGui::Separator();
	}

	ImGui::End();
}

DebugLogUI::~DebugLogUI()
{
}
