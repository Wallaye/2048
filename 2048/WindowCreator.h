#pragma once

#include "pch.h"
#include "framework.h"
#include "2048.h"

class WindowCreator
{
public:
	ATOM MyRegisterClass(HINSTANCE hInstance, WCHAR* szWindowClass, WNDPROC WndProc);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, WCHAR* szWindowClass, WCHAR* szTitle, RECT* rect);
	WindowCreator();
	~WindowCreator();
};
