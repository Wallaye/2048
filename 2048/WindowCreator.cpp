#include "pch.h"
#include "WindowCreator.h"
#include "WindowClass.h"

WindowCreator::WindowCreator() { };
WindowCreator::~WindowCreator() { };

ATOM WindowCreator::MyRegisterClass(HINSTANCE hInstance, WCHAR* szWindowClass, WNDPROC WndProc) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY2048));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
};

BOOL WindowCreator::InitInstance(HINSTANCE hInstance, int nCmdShow, WCHAR* szWindowClass, WCHAR* szTitle, RECT* rect, WindowClass* wc) {
    HWND hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPED | WS_BORDER | WS_SYSMENU | WS_VISIBLE,
        (GetSystemMetrics(SM_CXSCREEN) - rect->right) / 2, (GetSystemMetrics(SM_CYSCREEN) - rect->bottom) / 2,
        rect->right, rect->bottom, 0, 0, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    wc->hWnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
};
