#pragma once
#include "AUITab.h"

class DebugLogUI : public AUITab
{
public:
	DebugLogUI();
	~DebugLogUI();

	void RenderUI() override;

protected:
	void InitializeImGuiFlags() override;
};

