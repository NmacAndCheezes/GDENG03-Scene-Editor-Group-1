#include "AUITab.h"


AUITab::AUITab(std::string name) : name(name), isEnabled(true), flags(0)
{

}

AUITab::~AUITab()
{

}

std::string AUITab::GetName()
{
    return name;
}

void AUITab::SetName(std::string newName)
{
    name = newName;
}

bool AUITab::IsEnabled()
{
    return isEnabled;
}

void AUITab::SetEnabled(bool flag)
{
    isEnabled = flag;
}