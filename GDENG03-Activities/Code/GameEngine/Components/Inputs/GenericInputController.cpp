#include "GenericInputController.h"


GenericInputController::GenericInputController(std::string name) : AComponent(name, EComponentTypes::Input)
{

}

GenericInputController::~GenericInputController()
{

}

void GenericInputController::Clone(AComponent* copy)
{
	GenericInputController* copyInput = (GenericInputController*)copy;
	if (copyInput == nullptr) return;
	wParam = copyInput->wParam;
	lParam = copyInput->lParam;
}

void GenericInputController::ProcessInputs(WPARAM wParam, LPARAM lParam)
{
	this->wParam = wParam;
	this->lParam = lParam;
}

void GenericInputController::Perform()
{

}