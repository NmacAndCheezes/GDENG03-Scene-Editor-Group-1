#pragma once
#include <string>
#include <Windows.h>

class FileDialogs
{
public:
	FileDialogs(const FileDialogs&) = delete;
	FileDialogs& operator=(const FileDialogs&) = delete;

	static std::string OpenFile(const char* filter, HWND hWnd = nullptr);
	static std::string SaveFile(const char* filter, HWND hWnd = nullptr);

	static void SetDeafultHWND(HWND hWnd);

private:
	FileDialogs() {}

	static HWND staticHWnd;
};