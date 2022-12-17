#pragma once
#include <string>
#include "framework.h"

#define MAX_LOADSTRING 100

class FirstWindow
{
public:
	WCHAR szTitle[MAX_LOADSTRING];
	WCHAR szWindowClass[MAX_LOADSTRING];
	HINSTANCE hInst;
	void Start();
	FirstWindow();
	~FirstWindow();
private:

};
