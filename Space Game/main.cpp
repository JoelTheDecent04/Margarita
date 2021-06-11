#include <Windows.h>
#include <time.h>
#include <cassert>
#include "Game.h"
#include "Space.h"
#include "TitleScreen.h"
#include "Utilities.h"

const wchar_t* ClassName = L"WindowClass";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR lpCmdLine, int nCmdShow)
{
	srand(time(0));

	WNDCLASS wc = { 0 };
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = ClassName;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	ATOM ret = RegisterClass(&wc);
	assert(ret);

	RECT rect = { 0, 0, nScreenWidth, nScreenHeight };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(ClassName, L"Margarita 0.1.4", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);

	MSG msg;

	Graphics::Initialise(hwnd);
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Game::GameMain, nullptr, 0, nullptr);

	while (1)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		/*int nMonitorWidth = GetSystemMetrics(0);
		int nMonitorHeight = GetSystemMetrics(1);
		RECT rect = { 0, 0, nMonitorWidth, nMonitorHeight };
		bool b = InvalidateRect(Graphics::hWindow, &rect, false);*/
		return 0;
	}
	case WM_DESTROY:
		Game::Quit();
		while (1);
	case WM_LBUTTONDOWN:
		Game::LeftClick();
		return 0;
	case WM_KEYDOWN:
		if (GetForegroundWindow() == hwnd)
			Game::KeyDown(wParam);
		return 0;
	case WM_SIZE:
	{
			//Game::Resize();
			Graphics::mGraphics.lock();
			Graphics::Resize();
			Graphics::mGraphics.unlock();
			nScreenWidth = LOWORD(lParam);
			nScreenHeight = HIWORD(lParam);
			bool b = InvalidateRect(Graphics::hWindow, NULL, true);
		return 0;
	}
	case WM_MOUSEWHEEL:
		static int nDeltaMouseWheel = 0;
		nDeltaMouseWheel += GET_WHEEL_DELTA_WPARAM(wParam);
		while (nDeltaMouseWheel >= 120)
		{
			Game::KeyDown(VK_OEM_4);
			nDeltaMouseWheel -= 120;
		}
		while (nDeltaMouseWheel <= -120)
		{
			Game::KeyDown(VK_OEM_6);
			nDeltaMouseWheel += 120;
		}
		return 0;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}