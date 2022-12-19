#include "pch.h"
#include "RecordWindowHandle.h"
#include "WindowClass.h"

GdiPlusWorker* RecordWindowHandle::_worker;
std::map<int, std::wstring> RecordWindowHandle::records;

void RecordWindowHandle::TryAddRecord(WCHAR* str, int score)
{
	if (records.size() < 10)
		records.insert(std::pair<int, WCHAR*>(score, str));
	else {
		std::map<int, std::wstring>::iterator iterator = records.end();
		if (score > iterator->first) {
			records.erase(iterator);
			records.insert(std::pair<int, WCHAR*>(score, str));
		};
	}
}

void RecordWindowHandle::ReadRecords() {
	setlocale(LC_ALL, "Russian");
	std::locale::global(std::locale("Russian"));

	std::ifstream file(L"D:\\Records.txt");
	std::string buffer;
	while (std::getline(file, buffer)) {
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
		std::wstring a = utf8_conv.from_bytes(buffer);
		int index = a.rfind(L" ");
		std::wstring buff = a.substr(0, index);
		std::wstring _score = a.substr(index + 1);
		int score = std::stoi(_score);
		records.insert(std::pair<int, std::wstring>(score, buff));
	}
}

void RecordWindowHandle::SaveRecords()
{
	setlocale(LC_ALL, "Russian");
	std::locale::global(std::locale("Russian"));

	std::ofstream file(L"D:\\Records.txt");
	std::wstring buffer;
	for (auto it = records.rbegin(); it != records.rend(); it++) {
		static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
		file << utf8_conv.to_bytes(it->second);
		file << " ";
		file << std::to_string(it->first) << std::endl;
	}
}

WCHAR* text;
HFONT _NewFont = CreateFont(40, 16, 0, 0, 400, 0, 0, 0,
	DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_DONTCARE, L"Arial");
LRESULT RecordWindowHandle::RecordWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i = 0;
    switch (message)
    {
    case WM_COMMAND:
		if(LOWORD(wParam) == 10001) {
			ShowWindow(WindowClass::RecordWindow->hWnd, SW_HIDE);
			ShowWindow(WindowClass::WelcomeWindow->hWnd, SW_SHOW);
		}
        return 0;
    case WM_CREATE:
    {
		_worker = new GdiPlusWorker(hWnd);
		HWND hBtn = CreateWindowW(L"BUTTON", L"BACK",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			30, 25, 150, 50, hWnd, (HMENU)10001,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL);
		SendMessage(hBtn, WM_SETFONT, WPARAM(_NewFont), 0);
		_worker->FontCreate((WCHAR*)"Arial", 20);
    }
    return 0;
	case WM_PAINT:
		_worker->BeginScene();
		text = GetString();
		
		for (std::map<int, std::wstring>::reverse_iterator it = records.rbegin(); it != records.rend(); ++it) {
			WCHAR buff[64];
			lstrcpyW(buff, it->second.c_str());
			lstrcatW(buff, L" ");
			lstrcatW(buff, std::to_wstring(it->first).c_str());
			_worker->DrawString(buff, 0, PointF(10, 100 + 40 * i++), Color(0, 0, 0));
		}
		_worker->EndScene();
		return 0;
    case WM_SHOWWINDOW:
        InvalidateRect(hWnd, NULL, TRUE);
    return 0;
    case WM_CLOSE:
		//SaveRecords();
		DeleteObject(_NewFont);
        PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
		DeleteObject(_NewFont);
		//SaveRecords();
		break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
	return LRESULT();
}

WCHAR* RecordWindowHandle::GetString() {
	WCHAR result[512];
	int i = 0;
	for (std::map<int, std::wstring>::reverse_iterator it = records.rbegin(); it != records.rend(); ++it) {
		WCHAR buff[64];
		lstrcpyW(buff, it->second.c_str());
		lstrcatW(buff, L" ");
		lstrcatW(buff, std::to_wstring(it->first).c_str());
		lstrcatW(buff, L"\n");
		if (i == 0)
			lstrcpyW(result, buff);
		else 
			lstrcatW(result, buff);
		i++;
	}
	return result;
}