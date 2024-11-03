#include "Keyboard.h"


#pragma region Static-Members
std::unordered_map<unsigned char, bool> Keyboard::keyStates;
std::vector<Keyboard::KeyboardEvents> Keyboard::keyEventsBuffer;
std::vector<char> Keyboard::charBuffer;
#pragma endregion


#pragma region Public-Methods
bool Keyboard::IsKeyDown(unsigned char key)
{
	return keyStates[key];
}

bool Keyboard::IsKeyPressed(unsigned char key)
{
	for (int i = 0; i < keyEventsBuffer.size(); i++)
	{
		KeyboardEvents& keyEvent = keyEventsBuffer[i];
		if (keyEvent.Key == key && keyEvent.EventType == KeyboardEvents::Type::Pressed) return true;
	}

	return false;
}

bool Keyboard::IsKeyReleased(unsigned char key)
{
	for (int i = 0; i < keyEventsBuffer.size(); i++)
	{
		KeyboardEvents& keyEvent = keyEventsBuffer[i];
		if (keyEvent.Key == key && keyEvent.EventType == KeyboardEvents::Type::Released) return true;
	}

	return false;
}

std::vector<char>& Keyboard::GetCharBuffer()
{
	return charBuffer;
}

void Keyboard::FlushEventsBuffer()
{
	keyEventsBuffer.clear();
	keyEventsBuffer.shrink_to_fit();
}

void Keyboard::FlushCharBuffer()
{
	charBuffer.clear();
	charBuffer.shrink_to_fit();
}
#pragma endregion


#pragma region Input-Events
void Keyboard::OnKeyPressed(unsigned char key)
{
	keyStates[key] = true;

	for (int i = 0; i < keyEventsBuffer.size(); i++)
	{
		KeyboardEvents& keyEvent = keyEventsBuffer[i];
		if (keyEvent.Key == key && keyEvent.EventType == KeyboardEvents::Type::Pressed) return;
	}

	keyEventsBuffer.push_back({ key, KeyboardEvents::Type::Pressed });
}

void Keyboard::OnKeyReleased(unsigned char key)
{
	keyStates[key] = false;

	for (int i = 0; i < keyEventsBuffer.size(); i++)
	{
		KeyboardEvents& keyEvent = keyEventsBuffer[i];
		if (keyEvent.Key == key && keyEvent.EventType == KeyboardEvents::Type::Released) return;
	}

	keyEventsBuffer.push_back({ key, KeyboardEvents::Type::Released });
}

void Keyboard::OnChar(char character)
{
	charBuffer.push_back(character);
}
#pragma endregion
