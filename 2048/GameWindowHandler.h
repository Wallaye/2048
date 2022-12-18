#pragma once
#include "pch.h"
#include "GdiPlusWorker.h"
class GameWindowHandler
{
public:
	GameWindowHandler(int);
	~GameWindowHandler();
	void Draw();
	static LRESULT CALLBACK GameWindowWndProc(HWND, UINT, WPARAM, LPARAM);
private:
	GdiPlusWorker* _worker;
	RECT clientRect;
};