#include "Mouse.h"


#pragma region Static-Members
bool Mouse::isInWindow = false;
Vector2 Mouse::mousePos = Vector2::Zero;
int Mouse::wheelDeltaTotal = 0;
std::unordered_map<Mouse::EMouseButtons, bool> Mouse::buttonStates;
std::vector<Mouse::MouseEvents> Mouse::mouseEventsBuffer;
#pragma endregion


#pragma region Public-Methods
Vector2 Mouse::GetMousePos()
{
	return mousePos;
}

bool Mouse::IsMouseInWindow()
{
	return isInWindow;
}

bool Mouse::IsButtonDown(EMouseButtons button)
{
	return buttonStates[button];
}

bool Mouse::IsButtonPressed(EMouseButtons button)
{
	for (int i = 0; i < mouseEventsBuffer.size(); i++)
	{
		MouseEvents mEvent = mouseEventsBuffer[i]; 
		if ((int)button * 2 - 1 == (int)mEvent.EventType) return true;
	}
	return false;
}

bool Mouse::IsButtonReleased(EMouseButtons button)
{
	for (int i = 0; i < mouseEventsBuffer.size(); i++)
	{
		MouseEvents mEvent = mouseEventsBuffer[i];
		if ((int)button * 2 == (int)mEvent.EventType) return true;
	}
	return false;
}

int Mouse::GetMouseWheelRotations()
{
	int rotation = 0;

	for (int i = 0; i < mouseEventsBuffer.size(); i++)
	{
		MouseEvents mEvent = mouseEventsBuffer[i]; 
		if (mEvent.EventType == MouseEvents::Type::WheelDown) rotation--;
		else if (mEvent.EventType == MouseEvents::Type::WheelUp) rotation++;
	}

	return rotation;
}

void Mouse::FlushEventsBuffer()
{
	mouseEventsBuffer.clear();
	mouseEventsBuffer.shrink_to_fit();
}
#pragma endregion


#pragma region Input-Events
void Mouse::OnMouseMove(int x, int y)
{
	mousePos = Vector2((float)x, (float)y);
}

void Mouse::OnMouseChangeFocus(bool isInWindow, int x, int y)
{
	Mouse::isInWindow = isInWindow; 

	Vector2 pos = { (float)x, (float)y }; 
	MouseEvents::Type eType = (isInWindow) ? MouseEvents::Type::Enter : MouseEvents::Type::Leave;
	MouseEvents mEvent = { pos, eType }; 
	mouseEventsBuffer.push_back(mEvent); 
}

void Mouse::OnMousePress(EMouseButtons button, int x, int y)
{
	buttonStates[button] = true;

	Vector2 pos = { (float)x, (float)y };
	MouseEvents::Type eType = (MouseEvents::Type)((int)button * 2 - 1);
	MouseEvents mEvent = { pos, eType };
	mouseEventsBuffer.push_back(mEvent);
}

void Mouse::OnMouseRelease(EMouseButtons button, int x, int y)
{
	buttonStates[button] = false;

	Vector2 pos = { (float)x, (float)y };
	MouseEvents::Type eType = (MouseEvents::Type)((int)button * 2);
	MouseEvents mEvent = { pos, eType }; 
	mouseEventsBuffer.push_back(mEvent); 
}

void Mouse::OnMouseWheelRotate(int deltaWheel, int x, int y)
{
	wheelDeltaTotal += deltaWheel;
	Vector2 pos = { (float)x, (float)y }; 

	while (wheelDeltaTotal >= WHEEL_DELTA)
	{
		wheelDeltaTotal -= WHEEL_DELTA;
		MouseEvents mEvent = { pos, MouseEvents::Type::WheelUp }; 
		mouseEventsBuffer.push_back(mEvent); 
	}
	while (wheelDeltaTotal <= -WHEEL_DELTA)
	{
		wheelDeltaTotal += WHEEL_DELTA;
		MouseEvents mEvent = { pos, MouseEvents::Type::WheelDown }; 
		mouseEventsBuffer.push_back(mEvent); 
	}
}
#pragma endregion