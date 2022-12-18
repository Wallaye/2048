#pragma once
class GameWindowHandler
{
public:
	HDC hdcBack;
	HBITMAP hbmBack;
	void Draw(HDC);
	LRESULT CALLBACK GameWindowWndProc(HWND, UINT, WPARAM, LPARAM);
};

