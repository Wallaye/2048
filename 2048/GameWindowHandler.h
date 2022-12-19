#pragma once
#include "GdiPlusWorker.h"
#include "Game.h"


class GameWindowOptions {
public:
	RectF rect;
	RectF field;
	PointF scorePoint;
	int CellSize;
	int FieldSize;
};

class GameWindowHandler
{
public:
	RectF Rect;
	GameWindowOptions options;
	Game* game;
	bool EndedAlready = false;
	static GameWindowHandler* GetInstance();
	static LRESULT CALLBACK GameWindowWndProc(HWND, UINT, WPARAM, LPARAM);
private:
	GameWindowHandler();
	~GameWindowHandler();
	void Draw();
	static GameWindowHandler* _instance;
	GdiPlusWorker* _worker;
	RECT clientRect;
};