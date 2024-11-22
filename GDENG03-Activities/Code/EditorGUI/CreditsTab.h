#pragma once
#include "AUITab.h"
#include "DirectXClasses/Texture.h"

class CreditsTab : public AUITab
{
public: 
	CreditsTab();
	~CreditsTab();

	void RenderUI() override;

protected:
	void InitializeImGuiFlags() override;


private:
	Texture* dlsuLogo;
};