#include "pch.h"
#include "GdiPlusWorker.h"

GdiPlusWorker::GdiPlusWorker(HWND hWnd) : _hWnd(hWnd) {
	GetClientRect(_hWnd, &_size);
	_hdc = GetDC(_hWnd);
	_graphics = new Graphics(_hdc);
	_buffer = new Bitmap(_size.right, _size.bottom);
	_temp = new Graphics(_buffer);
}
GdiPlusWorker::~GdiPlusWorker() {
	delete(_graphics);
	delete(_temp);
	delete(_buffer);
	delete(_font);
	ReleaseDC(_hWnd, _hdc);
}

void GdiPlusWorker::BeginScene() {
	BeginPaint(_hWnd, &_ps);
}

void GdiPlusWorker::EndScene() {
	_graphics->DrawImage(_buffer, 0, 0, _size.right, _size.bottom);
	EndPaint(_hWnd, &_ps);
}

void GdiPlusWorker::FontCreate(WCHAR* fontName, REAL size, int style = FontStyleRegular, Unit unit = UnitPoint){
	_font = new Font(fontName, size, style, unit);
}

void GdiPlusWorker::DrawString(char* string, PointF coords, Color color, StringFormat* format = NULL) {
	int len = strlen(string);
	WCHAR* buff = new WCHAR[len + 1];
	buff[len] = 0;

	mbstowcs(buff, string, len);

	SolidBrush brush(color);
	if (format != NULL) {
		_temp->DrawString(buff, len, _font, coords, format, &brush);
	}
	else {
		_temp->DrawString(buff, len, _font, coords, &brush);
	}
	delete(buff);
}

void GdiPlusWorker::DrawImage(Image* image, RectF rect) {
	_temp->DrawImage(image, rect);
}

void GdiPlusWorker::DrawImage(HBITMAP picture, RectF rect) {
	Bitmap* bitmap = new Bitmap(picture, NULL);
	_temp->DrawImage(bitmap, rect);
	delete(bitmap);
}