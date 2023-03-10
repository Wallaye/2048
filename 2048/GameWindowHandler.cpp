#include "pch.h"
#include "GameWindowHandler.h"
#include "WindowClass.h"
#include "RecordWindowHandle.h"

Color colors[21] = {
    Color(195, 181, 169),
    Color(238, 228, 218),
    Color(237, 224, 200),
    Color(255, 179, 120),
    Color(255, 150, 96),
    Color(255, 125, 94),
    Color(255, 94, 54),
    Color(250, 208, 109),
    Color(244, 204, 91),
    Color(250, 200, 66),
    Color(254, 198, 47),
    Color(251, 196, 8),
    Color(255, 57, 61),
    Color(255, 23, 22),
    Color(255, 23, 22),
    Color(255, 23, 22),
    Color(255, 23, 22),
    Color(255, 23, 22),
    Color(255, 23, 22),
    Color(255, 23, 22),
    Color(255, 23, 22)
};

GameWindowHandler* GameWindowHandler::_instance;
GameWindowHandler::GameWindowHandler() {};
GameWindowHandler::~GameWindowHandler() { 
	delete(_worker);
};

GameWindowHandler* GameWindowHandler::GetInstance() {
    if (_instance == nullptr) {
        _instance = new GameWindowHandler();
    }
    return _instance;
};

void Move(DIRECTION);

GameWindowHandler* g;
HFONT __NewFont = CreateFont(40, 16, 0, 0, 400, 0, 0, 0,
    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE, L"Arial");
HWND hBtn;
LRESULT CALLBACK GameWindowHandler::GameWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    g = GameWindowHandler::GetInstance();
    DIRECTION direction;

    switch (message)
    {
    case WM_COMMAND:
        if (LOWORD(wParam) == 10001) {
            ShowWindow(WindowClass::WelcomeWindow->hWnd, SW_SHOW);
            DestroyWindow(hWnd);
        }
        return 0;
    case WM_KEYDOWN:
        if (g->game->IsGame) {
            switch (wParam) {
            case 0x41:	    //a
                direction = LEFT;
                Move(direction);
                InvalidateRect(hWnd, NULL, FALSE);
                break;
            case 0x44:		//d
                direction = RIGHT;
                Move(direction);
                InvalidateRect(hWnd, NULL, FALSE);
                break;
            case 0x53:		//s
                direction = BOTTOM;
                Move(direction);
                InvalidateRect(hWnd, NULL, FALSE);
                break;
            case 0x57:		//w
                direction = TOP;
                Move(direction);
                InvalidateRect(hWnd, NULL, FALSE);
                break;
            }
            g->game->IsGame = g->game->CheckForEnd();
            if (!g->game->IsGame) {
                HWND hBtn = CreateWindowW(L"BUTTON", L"BACK",
                    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                    10, 50, 120, 50, hWnd, (HMENU)10001,
                    (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
                    NULL);
                SendMessage(hBtn, WM_SETFONT, WPARAM(__NewFont), 0);
            }
            if (g->game->size == 4) {
                if (!g->game->IsGame) {
                    RecordWindowHandle::TryAddRecord(g->game->playerName, g->game->score);
                    RecordWindowHandle::SaveRecords();
                }
            }
        }
        else return DefWindowProc(hWnd, message, wParam, lParam); 
        return 0;
    case WM_CREATE:
    {
        g->_worker = new GdiPlusWorker(hWnd);
        g->_worker->FontCreate((WCHAR*)"Arial", 24);
        g->_worker->FontCreate((WCHAR*)"Arial", 32);
        g->_worker->FontCreate((WCHAR*)"Arial", 16);
    }
    return 0;
    case WM_SHOWWINDOW:
        InvalidateRect(hWnd, NULL, TRUE);
    case WM_PAINT:
    {
        g->_worker->BeginScene();
        g->Draw();
        g->_worker->EndScene();
    }
    return 0;
    case WM_CLOSE:
        DeleteObject(__NewFont);
        PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
        DeleteObject(__NewFont);
        delete WindowClass::GameWindow;
        delete GameWindowHandler::_instance->game;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Move(DIRECTION direction) {
    g->game->canMakeMove = g->game->MakeMove(direction);
    if (g->game->canMakeMove) {
        g->game->GenerateNumber();
    }
}

void GameWindowHandler::Draw() {
    const int CellSize = g->options.CellSize;
    const int BorderSize = 10;
    RectF rect;
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    g->_worker->FillRect(g->options.rect, Color(240, 240, 240));
    g->_worker->FillRect(g->options.field, Color(159, 136, 117));
    g->_worker->DrawString((char*)"SCORE:", 1, g->options.scorePoint, Color(0, 0, 0));
    g->_worker->DrawString(g->game->playerName, 2, PointF(10, 10), Color(123, 123, 123));
    char buff[10];
    _itoa_s(game->score, buff, 10);
    PointF score = g->options.scorePoint;
    score.Y += 25;
    score.X += 240;
    g->_worker->DrawString(buff, 1, score, Color(0, 0, 0), &format);
    for (int i = 0; i < g->game->size; i++) {
        for (int j = 0; j < g->game->size; j++) {
            rect.X = 10 + (j + 1) * BorderSize + j * CellSize;
            rect.Y = 140 + (i + 1) * BorderSize + i * CellSize;
            rect.Width = rect.Height = CellSize;
            g->_worker->FillRect(rect, colors[g->game->field[i][j]]);
            PointF point(rect.X + CellSize / 2, rect.Y + CellSize / 2);
            g->_worker->DrawString((char*)g->game->array_of_accordance[g->game->field[i][j]].str,
                0, point, Color(0, 0, 0), &format);
        }
    }
    if (!g->game->IsGame) {
        PointF point = g->options.scorePoint;
        point.Y -= 50;
        if (g->game->FindMaxPower() < g->game->MaxPower) {
            g->_worker->DrawString((char*)"YOU LOST!", 1, point, Color(255, 0, 0));
        }
        else {
            g->_worker->DrawString((char*)"YOU WON!", 1, point, Color(0, 255, 0));
        }
    }
}