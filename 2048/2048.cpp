#include "pch.h"
#include "framework.h"
#include "2048.h"
#include "WindowCreator.h"
#include <CommCtrl.h>
#include "WindowClass.h"
#include "GdiPlusWorker.h"
#include "GameWindowHandler.h"
#include "RecordWindowHandle.h"

#    pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
#define START_GAME_BTN 10000
#define RECORDS_GAME_BTN 10002

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];      
WCHAR szGameWindowClass[MAX_LOADSTRING];
WCHAR szRecordsWindowClass[MAX_LOADSTRING];
HFONT NewFont = CreateFont(40, 16, 0, 0, 400, 0, 0, 0,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, L"Arial");
RECT clientRect;
WindowClass *WelcomeWindow, *GameWindow, *RecordWindow;
int _nCmdShow;

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
    LoadStringW(hInstance, IDC_GAMEWINDOW, szGameWindowClass, MAX_LOADSTRING);
    LoadStringW(hInstance, IDS_RECORDSWINDOW, szRecordsWindowClass, MAX_LOADSTRING);

    RECT rect = { 0, 0, 600, 500 };
    WelcomeWindow = new WindowClass(hInstance, WndProc, szTitle, szWindowClass, nCmdShow, rect);
    WindowClass::WelcomeWindow = WelcomeWindow;
    hInst = hInstance;
    _nCmdShow = nCmdShow;
    if (!WelcomeWindow->create()) {
        return FALSE;
    };
    ShowWindow(WelcomeWindow->hWnd, WelcomeWindow->nCmdShow);
    UpdateWindow(WelcomeWindow->hWnd);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048));

    MSG msg;
    
    RecordWindowHandle::ReadRecords();

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

//WelcomeWindow WNDPROC
GameWindowOptions options[4]{
    {{0, 0, 500, 650}, {10, 140, 460, 465}, {150, 50}, 140, 10},
    {{0, 0, 600, 750}, {10, 140, 560, 565}, {250, 50}, 127, 10},
    {{0, 0, 690, 850}, {10, 140, 660, 665}, {300, 50}, 120, 10},
    {{0, 0, 820, 980}, {10, 140, 790, 795}, {350, 50}, 120, 10},
};
WCHAR ComboBoxTexts[4][4] = {
    TEXT("3x3"), TEXT("4x4"), TEXT("5x5"), TEXT("6x6")
};
WCHAR A[4];
HWND hCmb, hEdit, hBtnStart, hBtnRecords;
int index = 1;
HDC hdc;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    GetClientRect(hWnd, &clientRect);
    switch (message)
    {
    case WM_COMMAND:
        if (HIWORD(wParam) == CBN_SELCHANGE) {
            HWND const control{ (HWND)lParam };
            int ind = SendMessageW(control, CB_GETCURSEL, 0, 0);
            if (ind != CB_ERR) {
                index = ind;
            }
            DeleteObject(control);
        }
        if (LOWORD(wParam) == START_GAME_BTN) {
            int length = GetWindowTextLength(hEdit);
            if (length < 1) {
                MessageBoxW(hWnd, L"Enter UserName", L"FAIL", MB_OK);
                return 0;
            }
            length++;
            WCHAR str[32];
            GetWindowTextW(hEdit, str, length);
            GameWindowHandler* gwh = GameWindowHandler::GetInstance();
            gwh->game = new Game(index + 3, str);
            gwh->options = options[index];
            RECT rect = { gwh->options.rect.X, gwh->options.rect.Y, gwh->options.rect.Width, gwh->options.rect.Height };
            GameWindow = new WindowClass(hInst, GameWindowHandler::GameWindowWndProc, szGameWindowClass, (WCHAR*)L"Game", _nCmdShow, rect);
            GameWindow->create();
            WindowClass::GameWindow = GameWindow;
            ShowWindow(GameWindow->hWnd, GameWindow->nCmdShow);
            UpdateWindow(GameWindow->hWnd);
            ShowWindow(WelcomeWindow->hWnd, SW_HIDE); //Hide welcome window
            //delete[] str;
        }
        else if (LOWORD(wParam) == RECORDS_GAME_BTN) {
            if (RecordWindow == NULL) {
                RECT rect = { 0, 0, 350, 600 };
                RecordWindow = new WindowClass(hInst, RecordWindowHandle::RecordWndProc, szRecordsWindowClass, (WCHAR*)L"Records", _nCmdShow, rect);
                RecordWindow->create();
                WindowClass::RecordWindow = RecordWindow;
            }
            ShowWindow(RecordWindow->hWnd, RecordWindow->nCmdShow);
            UpdateWindow(RecordWindow->hWnd);
            ShowWindow(WelcomeWindow->hWnd, SW_HIDE);
        }
        return 0;
    case WM_CREATE:
    {
        //Creating Edit for inputing player name
        hEdit = CreateWindow(WC_EDIT, L"player",
            WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE | ES_CENTER,
            150, 80, 300, 60, hWnd, NULL,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        //Creating ComboBox with levels of difficulty
        hCmb = CreateWindow(WC_COMBOBOX, TEXT(""),
            CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
            200, 200, 200, 200, hWnd, NULL,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        //Creating Button Starting the game
        hBtnStart = CreateWindowW(WC_BUTTON, L"START",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            200, 290, 200, 60, hWnd, (HMENU)START_GAME_BTN,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        //Creating Button showing the records
        hBtnRecords = CreateWindowW(WC_BUTTON, L"RECORDS",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            200, 370, 200, 60, hWnd, (HMENU)RECORDS_GAME_BTN,
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