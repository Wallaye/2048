#include "pch.h"
#include "WindowClass.h"

WindowClass* WindowClass::WelcomeWindow;
WindowClass* WindowClass::GameWindow;
WindowClass* WindowClass::RecordWindow;

WindowClass::WindowClass(HINSTANCE hInst, WNDPROC proc, WCHAR* title, WCHAR* name, int cmd, RECT rect) {
	this->hInstance = hInst;
	this->WndProc = proc;
	this->szTitle = title;
	this->szWindowClass = name;
	this->nCmdShow = cmd;
	this->rect = rect;
}
WindowClass::~WindowClass() {};

bool WindowClass::create() {
	WindowCreator WC;
	WC.MyRegisterClass(hInstance, szWindowClass, WndProc);
	bool result = WC.InitInstance(hInstance, nCmdShow, szWindowClass, szTitle, &rect, this);
	return result;
}