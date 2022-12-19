#pragma once
#include "pch.h"
#include "GdiPlusWorker.h"
#include "2048.h"


class GameWindowHandler
{
public:
	GameWindowHandler(int, WCHAR*);
	~GameWindowHandler();
	int CellSize;
	int BorderSize;
	RectF Rect;
	GameWindowOptions options;
	void Draw();
	static LRESULT CALLBACK GameWindowWndProc(HWND, UINT, WPARAM, LPARAM);
private:
	GdiPlusWorker* _worker;
	RECT clientRect;
};