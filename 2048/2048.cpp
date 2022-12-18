#include "pch.h"
#include "framework.h"
#include "2048.h"
#include "WindowCreator.h"
#include <CommCtrl.h>
#include "WindowClass.h"

#define MAX_LOADSTRING 100
#define START_GAME_BTN 10000

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];       
HFONT NewFont = CreateFont(40, 16, 0, 0, 400, 0, 0, 0,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, L"Arial");
RECT clientRect;
WindowClass *WelcomeWindow, *GameWindow;

BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Gdiplus::GdiplusStartupInput gdiPlusStartupInput;
    ULONG_PTR gdiPlusToken;
    Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiPlusStartupInput, NULL);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2048, szWindowClass, MAX_LOADSTRING);

    RECT rect = { 0, 0, 600, 500 };
    WelcomeWindow = new WindowClass(hInstance, (WNDPROC)WndProc, szTitle, szWindowClass, nCmdShow, rect);
    if (!WelcomeWindow->create()) {
        return FALSE;
    };
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    Gdiplus::GdiplusShutdown(gdiPlusToken);

    return (int) msg.wParam;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
    WindowCreator WC;
    RECT rect = { 0, 0, 600, 500 };
    return WC.InitInstance(hInstance, nCmdShow, szWindowClass, szTitle, &rect);
}

WCHAR ComboBoxTexts[4][4] = {
    TEXT("3x3"), TEXT("4x4"), TEXT("5x5"), TEXT("6x6")
};
WCHAR A[4];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    GetClientRect(hWnd, &clientRect);
    HDC hdc = GetDC(hWnd);

    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == START_GAME_BTN) {
            GameWindow = new WindowClass();
        }
        return 0;
    case WM_CREATE:
        {
            HWND hEdit = CreateWindow(WC_EDIT, L"player",
                WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER,
                150, 80, 300, 60, hWnd, NULL,
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);
            //Creating ComboBox with level of difficulty
            HWND hCmb = CreateWindow(WC_COMBOBOX, TEXT(""),
                CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                200, 200, 200, 200, hWnd, NULL,
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);
            //Creating Button Starting the game
            HWND hBtnStart = CreateWindowW(WC_BUTTON, L"START",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                200, 290, 200, 60, hWnd, (HMENU)START_GAME_BTN,
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);
            HWND hBtnRecords = CreateWindowW(WC_BUTTON, L"RECORDS",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                200, 370, 200, 60, hWnd, NULL,
                (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                NULL);
            memset(&A, 0, sizeof(A));
            for (int i = 0; i < 4; i++) {
                wcscpy_s(A, sizeof(A) / sizeof(TCHAR), (TCHAR*)ComboBoxTexts[i]);
                SendMessage(hCmb, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A);
            }

            SendMessage(hBtnStart, WM_SETFONT, WPARAM(NewFont), 0);
            SendMessage(hBtnRecords, WM_SETFONT, WPARAM(NewFont), 0);
            SendMessage(hCmb, WM_SETFONT, WPARAM(NewFont), 0);
            SendMessage(hEdit, WM_SETFONT, WPARAM(NewFont), 0);
            SendMessage(hCmb, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
            InvalidateRect(hWnd, NULL, TRUE);
        }
    return 0;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            SaveDC(hdc);
            HBRUSH brush = CreateSolidBrush(RGB(214, 252, 212));
            SelectObject(hdc, brush);
            HDC hdc = BeginPaint(hWnd, &ps);
            FillRect(hdc, &clientRect, brush);
            SelectObject(hdc, NewFont);
            RECT rect = { 150, 150, 450, 200 };
            WCHAR buff[20] = L"Choose difficulty";
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc, buff, -1, &rect, DT_CENTER);
            rect = { 150, 20, 450, 70 };
            wcscpy_s(buff, L"Enter your name");
            DrawText(hdc, buff, -1, &rect, DT_CENTER);
            RestoreDC(hdc, -1);
            DeleteObject(brush);
            EndPaint(hWnd, &ps);
        }
        return 0;
    case WM_DESTROY:
        DeleteObject(NewFont);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

