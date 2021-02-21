#pragma once
#include <Windows.h>
#include "MouseClass.h"
class Window
{
public:
	int init(HINSTANCE hInstance, INT width, INT height, int nCmdShow);
	int WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void initMouse();
	void Update();
	bool run();
	void deInit();

	HWND getWindowHandle() { return _hWnd; }
private:
	HWND _hWnd = nullptr;
protected:
	MouseClass _mouse;
};

