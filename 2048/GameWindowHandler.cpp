#include "pch.h"
#include "GameWindowHandler.h"

GameWindowHandler::GameWindowHandler(HWND hWnd) {
	_worker = new GdiPlusWorker(hWnd);
};
GameWindowHandler::~GameWindowHandler() { 
	delete(_worker);
};