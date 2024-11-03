#pragma once
#include <SimpleMath.h>
#include <vector>
#include <unordered_map>

using namespace DirectX::SimpleMath; 

class Mouse
{
#pragma region Mouse-Data-Types
public:
	enum EMouseButtons
	{
		Unknown = 0,
		Left,
		Right,
		Middle
	};

private:
	struct MouseEvents
	{
	public:
		enum class Type
		{
			Unknown = 0,
			LeftPressed,
			LeftReleased,
			RightPressed,
			RightReleased,
			MiddlePressed,
			MiddleReleased,
			WheelUp,
			WheelDown,
			Enter,
			Leave
		};

	public:
		MouseEvents(Vector2 currentPos, Type type) : CurrentPos(currentPos), EventType(type) {}

	public:
		Vector2 CurrentPos;
		Type EventType;
	};
#pragma endregion


public:
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	static Vector2 GetMousePos();
	static bool IsMouseInWindow();

	static bool IsButtonDown(EMouseButtons button);
	static bool IsButtonPressed(EMouseButtons button);
	static bool IsButtonReleased(EMouseButtons button);

	static int GetMouseWheelRotations();
	static void FlushEventsBuffer();

private:
	Mouse() {};

	static void OnMouseMove(int x, int y);
	static void OnMouseChangeFocus(bool isInWindow, int x, int y);
	static void OnMousePress(EMouseButtons button, int x, int y);
	static void OnMouseRelease(EMouseButtons button, int x, int y);
	static void OnMouseWheelRotate(int deltaWheel, int x, int y);

private:
	static bool isInWindow; 
	static Vector2 mousePos; 
	static int wheelDeltaTotal;
	static std::unordered_map<EMouseButtons, bool> buttonStates;
	static std::vector<MouseEvents> mouseEventsBuffer;

	friend class Window;
};