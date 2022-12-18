#pragma once
#include<gdiplus.h>

using namespace Gdiplus;
class GdiPlusWorker
{
public:
	GdiPlusWorker(HWND hWnd);
	~GdiPlusWorker();
	void BeginScene();
	void EndScene();
	void DrawString(WCHAR*, PointF, Color, StringFormat* format = NULL);
	void FontCreate(WCHAR*, REAL, int style = FontStyleRegular, Unit unit = UnitPoint);
	
	void DrawImage(Image* image, RectF rect);
	void DrawImage(HBITMAP picture, RectF rect);
private:
	Graphics* _graphics;
	Graphics* _temp;
	Bitmap* _buffer;
	Font* _font;

	HDC _hdc;
	HWND _hWnd;
	PAINTSTRUCT _ps;

	RECT _size;
};

