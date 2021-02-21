#include "Window.h"
#include<string>
//Receives Windows procedures and interacts with them
int Window:: WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		//sends a message to close the program when closing the window or pressing Escape
	case WM_CLOSE:

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		return 0;

	
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}

void Window::initMouse()
{
	_mouse = MouseClass();
}

LRESULT CALLBACK HandleMsgRedirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;

	default:
	{
		// get pointer to window class
		Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// forward message to window class handler
		return pWindow->WndProc(hWnd, msg, wParam, lParam);
	}
	}
}

//Initialises the Window, difining its size and position
int Window::init(HINSTANCE hInstance, INT width, INT height, int nCmdShow)
{
	WNDCLASS wc = {};
	wc.hInstance = hInstance;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND + 1);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.lpszClassName = TEXT("Direct3D 11");
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMsgRedirect;
	
	if (!RegisterClass(&wc)) return 10;

	INT halfScreenWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	INT halfScreenHeight = GetSystemMetrics(SM_CYSCREEN) / 2;
	INT halfWidth = width / 2;
	INT halfHeight = height / 2;
	RECT r{ halfScreenWidth - halfWidth, halfScreenHeight - halfHeight,
			halfScreenWidth + halfWidth, halfScreenHeight + halfHeight };
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&r, style, false);

	_hWnd = CreateWindow(wc.lpszClassName, wc.lpszClassName, style,
		r.left, r.top, r.right - r.left, r.bottom - r.top, nullptr, nullptr, hInstance, nullptr);

	//return 15 if there was an error while creating the window
	if (!_hWnd) return 15;

	ShowWindow(_hWnd, nCmdShow);
	SetFocus(_hWnd);

	return 0;
}
void Window::Update()
{
	while (!_mouse.EventBufferIsEmpty())
	{
		MouseEvent me = _mouse.ReadEvent();
		// for testing
		std::string outmsg = "X: ";
		outmsg += std::to_string(me.GetPosX());
		outmsg += ", Y: ";
		outmsg += std::to_string(me.GetPosY());
		OutputDebugStringA(outmsg.c_str());
	}
}
bool Window::run()
{
	MSG msg = {};
	if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) return false;
	}

	return true;
}


void Window::deInit()
{
	// TODO: destroy window if it is not destroyed
}


