#include "AUIScreen.h"

AUIScreen::AUIScreen()
{
}

AUIScreen::~AUIScreen()
{
}

void AUIScreen::setIsActive(bool isActive)
{
	this->isActive = isActive;
}

bool AUIScreen::getIsActive()
{
	return isActive;
}
