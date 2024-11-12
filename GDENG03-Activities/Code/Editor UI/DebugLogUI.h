#pragma once
#include "AUIScreen.h"

class DebugLogUI : public AUIScreen
{
public:
	DebugLogUI();
	virtual void update() override;
	~DebugLogUI();
};

