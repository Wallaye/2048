#pragma once
#include "WindowCreator.h"
#include <vector>

class WindowClass
{
public:
	HINSTANCE hInstance;
	WNDPROC WndProc;
	WCHAR* szTitle;
	WCHAR* szWindowClass;
	int nCmdShow;
	RECT rect;
	HWND hWnd;
	bool create();
	WindowClass(HINSTANCE, WNDPROC, WCHAR*, WCHAR*, int, RECT);
	~WindowClass();
	static WindowClass* WelcomeWindow;
	static WindowClass* GameWindow;
	static WindowClass* RecordWindow;
private:
};

