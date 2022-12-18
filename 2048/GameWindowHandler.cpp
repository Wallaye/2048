#include "pch.h"
#include "GameWindowHandler.h"
#include "Game.h"

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
Game* game;
GameWindowHandler::GameWindowHandler(int size) {
    game = new Game(size);
};
GameWindowHandler::~GameWindowHandler() { 
	delete(_worker);
};

GameWindowHandler* g;
void Move(DIRECTION);

LRESULT CALLBACK GameWindowHandler::GameWindowWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    GetClientRect(hWnd, &g->clientRect);
    HDC hdc = GetDC(hWnd);
    DIRECTION direction;

    switch (message)
    {
    case WM_COMMAND:
        return 0;
    case WM_KEYDOWN:
        if (game->IsGame) {
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
            game->IsGame = game->CheckForEnd();
        }
        else return DefWindowProc(hWnd, message, wParam, lParam); 
        return 0;
    case WM_CREATE:
    {
        g = new GameWindowHandler(game->size);
        g->_worker = new GdiPlusWorker(hWnd);
        g->_worker->FontCreate((WCHAR*)"Arial", 24);
        g->_worker->FontCreate((WCHAR*)"Arial", 32);
        InvalidateRect(hWnd, NULL, TRUE);
    }
    return 0;
    case WM_PAINT:
    {
        g->_worker->BeginScene();
        g->Draw();
        g->_worker->EndScene();
    }
    return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Move(DIRECTION direction) {
    game->canMakeMove = game->MakeMove(direction);
    if (game->canMakeMove) {
        game->GenerateNumber();
    }
}

const int CellSize = 127;
const int BorderSize = 10;
void GameWindowHandler::Draw() {
    RectF rect;
    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);
    g->_worker->FillRect(RectF(0, 0, 600, 750), Color(240, 240, 240));
    g->_worker->FillRect(RectF(10, 140, 560, 565), Color(159, 136, 117));
    g->_worker->DrawString((char*)"SCORE:", 1, PointF(250, 50), Color(0, 0, 0));
    char buff[10];
    _itoa_s(game->score, buff, 10);
    g->_worker->DrawString(buff, 1, PointF(480, 75), Color(0, 0, 0), &format);
    for (int i = 0; i < game->size; i++) {
        for (int j = 0; j < game->size; j++) {
            rect.X = 10 + (j + 1) * BorderSize + j * CellSize;
            rect.Y = 140 + (i + 1) * BorderSize + i * CellSize;
            rect.Width = rect.Height = CellSize;
            g->_worker->FillRect(rect, colors[game->field[i][j]]);
            PointF point(rect.X + CellSize / 2, rect.Y + CellSize / 2);
            g->_worker->DrawString((char*)game->array_of_accordance[game->field[i][j]].str,
                0, point, Color(0, 0, 0), &format);
        }
    }
}