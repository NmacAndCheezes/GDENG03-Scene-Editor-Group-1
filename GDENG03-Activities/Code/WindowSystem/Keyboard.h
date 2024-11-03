#pragma once
#include <unordered_map>
#include <vector>

class Keyboard
{
#pragma region Keyboard-Data-Types
private:
	struct KeyboardEvents
	{
	public:
		enum class Type
		{
			Unknown,
			Pressed,
			Released
		};

	public:
		KeyboardEvents(unsigned char key, Type type) : Key(key), EventType(type) {}

	public:
		unsigned char Key;
		Type EventType;
	};
#pragma endregion


public:
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	static bool IsKeyDown(unsigned char key);
	static bool IsKeyPressed(unsigned char key);
	static bool IsKeyReleased(unsigned char key);
	static std::vector<char>& GetCharBuffer();

	static void FlushEventsBuffer();
	static void FlushCharBuffer();

private:
	Keyboard() {};

	static void OnKeyPressed(unsigned char key);
	static void OnKeyReleased(unsigned char key);
	static void OnChar(char character);


private:
	static std::unordered_map<unsigned char, bool> keyStates;
	static std::vector<KeyboardEvents> keyEventsBuffer;
	static std::vector<char> charBuffer;

	friend class Window;
};