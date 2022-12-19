#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>
#include "GdiPlusWorker.h"

class RecordWindowHandle
{
public:
	static GdiPlusWorker* _worker;
	static std::map<int, std::wstring> records;
	static void TryAddRecord(WCHAR*, int);
	static WCHAR* GetString();
	static void ReadRecords();
	static void SaveRecords();
	static LRESULT CALLBACK RecordWndProc(HWND, UINT, WPARAM, LPARAM);
};
