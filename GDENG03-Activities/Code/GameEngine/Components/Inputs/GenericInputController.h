#pragma once
#include <Windows.h>
#include "../AComponent.h"


class GenericInputController : public AComponent
{
public:
	GenericInputController(std::string name);
	~GenericInputController();

	void Clone(AComponent* copy) override;
	void ProcessInputs(WPARAM wParam, LPARAM lParam);
	void Perform() override;


protected:
	WPARAM wParam;
	LPARAM lParam;
};