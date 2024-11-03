#include "Window.h"
#include "../WindowsMessageMap.h"
#include "../GameEngine/EngineTime.h"
#include "../Editor UI/imgui-master/imgui.h"


Window::Window() : width (640), height(480), isRunning(false),
	windowClassName("WindowClass"), windowStyles(WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU), parentHwnd(nullptr), hWnd(nullptr)
{

}

Window::~Window()
{

}

bool Window::Init(int width, int height, std::string windowName)
{
	this->width = width;
	this->height = height;

	WNDCLASSEX wClass = {};
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.style = CS_OWNDC;
	wClass.lpfnWndProc = StaticWndProc; 
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hInstance = GetModuleHandleA(nullptr);
	wClass.hCursor = nullptr;
	wClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);;
	wClass.hbrBackground = nullptr;
	wClass.lpszMenuName = nullptr;
	wClass.lpszClassName = windowClassName.c_str();

	if (!RegisterClassEx(&wClass))
	{
		return false;
	}

	windowSize.left = 100;
	windowSize.right = width + windowSize.left;
	windowSize.top = 100;
	windowSize.bottom = height + windowSize.top;
	if (AdjustWindowRect(&windowSize, windowStyles, FALSE) == 0)
	{
		return false;
	}
	
	if (!(hWnd = CreateWindowEx(0, windowClassName.c_str(), windowName.c_str(), windowStyles, 
		CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right - windowSize.left, windowSize.bottom - windowSize.top,
		parentHwnd, nullptr, GetModuleHandleA(nullptr), this)))
	{
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	isRunning = true;

    return true;
}

bool Window::Release()
{
	if (!UnregisterClass(windowClassName.c_str(), GetModuleHandleA(nullptr)))
	{
		// uncomment for DEBUG
		//TCHAR   lpBuffer[256] = TEXT("?");
		//FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,           // It´s a system error
		//		NULL,                                       // No string to be formatted needed
		//		GetLastError(),                             // Hey Windows: Please explain this error!
		//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // Do it in the standard language
		//		lpBuffer,                                   // Put the message here
		//		255,                                        // Number of bytes to store the message
		//		NULL);
		//OutputDebugString(lpBuffer);
		return false;
	}

	// most likely Release() is called when window is already closed, so destroy might be unnecessary
	/*if (!DestroyWindow(hWnd))
	{
		return false;
	}*/

	return true;
}

bool Window::Update()
{
	EngineTime::LogStartFrame();
	this->OnUpdate();

	MSG msg = {  };
	while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) > 0)
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	EngineTime::LogEndFrame();

	return true;
}

bool Window::IsRunning()
{
	return isRunning;
}

RECT Window::GetClientWindowRect()
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	return rc;
}

void Window::OnDestroy()
{
	isRunning = false;
}

  extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Window::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return true;
	}
	// uncomment for DEBUG (also prints the Widnow Messages (msg) to the Output Tab)
	//static WindowsMessageMap mm;
	//OutputDebugString(mm(msg, lParam, wParam).c_str());

	Window* window = nullptr;

	switch (msg)
	{
		case WM_CREATE:
		{
			window = static_cast<Window*>((reinterpret_cast<LPCREATESTRUCT>(lParam))->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
			window->OnCreate(hWnd);
			break;
		}
		case WM_DESTROY:
		{
			window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			window->OnDestroy(); 
			PostQuitMessage(0); 
			break; 
		}
		default:
		{
			window = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			if (window == nullptr) return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}
	}

	return window->HandleWindowMessages(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleWindowMessages(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		#pragma region Keyboard-Events
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000)) Keyboard::OnKeyPressed(static_cast<unsigned char>(wParam)); 
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			if (wParam == VK_ESCAPE)
			{
				OnDestroy();
				PostQuitMessage(0);
				break;
			}

			Keyboard::OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		}

		case WM_CHAR:
			Keyboard::OnChar(static_cast<unsigned char>(wParam)); 
			break;
		#pragma endregion

		#pragma region Mouse-Events
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam); 

			// if mouse is within screen of window
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
			{
				Mouse::OnMouseMove(pt.x, pt.y);
				
				// if mouse was outside window, then call 'mouse enter' event
				if (!Mouse::isInWindow) 
				{
					SetCapture(hWnd);
					Mouse::OnMouseChangeFocus(true, pt.x, pt.y);
				}
			}
			else
			{
				int numMouseButtons = 3;
				bool hasOneButtonPressed = false;
				for (int i = 1; i < numMouseButtons; i++)
				{
					hasOneButtonPressed = hasOneButtonPressed || Mouse::buttonStates[(Mouse::EMouseButtons)i];
				}

				// if at least one mouse button is down, then keep updating mouse position
				if (hasOneButtonPressed)
				{
					Mouse::OnMouseMove(pt.x, pt.y); 
				}
				else
				{
					ReleaseCapture();
					Mouse::OnMouseChangeFocus(false, pt.x, pt.y);
				}
			}

			break;
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			Mouse::OnMousePress(Mouse::EMouseButtons::Left, pt.x, pt.y); 
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			Mouse::OnMousePress(Mouse::EMouseButtons::Right, pt.x, pt.y); 
			break;
		}
		case WM_MBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			Mouse::OnMousePress(Mouse::EMouseButtons::Middle, pt.x, pt.y);  
			break;
		}
		case WM_LBUTTONUP:
		{
			OnMouseRelease(lParam, Mouse::EMouseButtons::Left); 
			break;
		}
		case WM_RBUTTONUP:
		{
			OnMouseRelease(lParam, Mouse::EMouseButtons::Right); 
			break;
		}
		case WM_MBUTTONUP: 
		{
			OnMouseRelease(lParam, Mouse::EMouseButtons::Middle); 
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam); 
			Mouse::OnMouseWheelRotate(GET_WHEEL_DELTA_WPARAM(wParam), pt.x, pt.y); 
			break;
		}
		#pragma endregion
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Window::OnMouseRelease(LPARAM lParam, Mouse::EMouseButtons button)
{
	const POINTS pt = MAKEPOINTS(lParam);
	Mouse::OnMouseRelease(button, pt.x, pt.y);

	if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
	{
		ReleaseCapture();
		Mouse::OnMouseChangeFocus(false, pt.x, pt.y);
	}
}