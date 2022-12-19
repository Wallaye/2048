#pragma once
#include<gdiplus.h>
#include<vector>

using namespace Gdiplus;
class GdiPlusWorker
{
public:
	GdiPlusWorker(HWND hWnd);
	~GdiPlusWorker();
	void BeginScene();
	void EndScene();
	void DrawString(char*, int, PointF, Color, StringFormat* format = NULL);
	void FontCreate(WCHAR*, REAL, int style = FontStyleRegular, Unit unit = UnitPoint);
	void DrawImage(Image* image, RectF rect);
	void DrawImage(HBITMAP picture, RectF rect);
	void FillRect(RectF, Color);
private:
	Graphics* _graphics;
	Graphics* _temp;
	Bitmap* _buffer;
	std::vector<Font*> _fonts;

	HDC _hdc;
	HWND _hWnd;
	PAINTSTRUCT _ps;

	RECT _size;
};

