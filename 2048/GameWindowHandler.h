#pragma once
#include "pch.h"
#include "GdiPlusWorker.h"
class GameWindowHandler
{
public:
	GameWindowHandler(HWND hWnd);
	~GameWindowHandler();
	HDC hdcBack;
	HBITMAP hbmBack;
	void Draw(HDC);
	LRESULT CALLBACK GameWindowWndProc(HWND, UINT, WPARAM, LPARAM);
private:
	GdiPlusWorker* _worker;
};

