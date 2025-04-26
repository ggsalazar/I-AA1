#include <windowsx.h>
#include <ShellScalingApi.h>
#include "Window_Windows.h"
#pragma comment(lib, "Shcore.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool Window_Windows::Create(const char* title, Vector2u size) {

	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = L"AetherEngineWindowClass";

	RegisterClass(&wc);

	//Get the information of the specified monitor
	auto monitors = EnumerateMonitors();
	monitor = monitor >= monitors.size() ? 0 : monitor;
	RECT r = monitors[monitor].rect;

	

	//Default to fullscreen
	if (size == Vector2u{0, 0})
		size = { (uint)(r.right - r.left), (uint)(r.bottom - r.top) };
	//If passed size is too big, render at double the minimum resolution
	if (size.x > (r.right - r.left) or size.y > (r.bottom - r.top))
		size = Vector2u{ 1280, 720 };

	win_size = size;

	//Convert the title from a const char* to a wide string
	std::wstring wtitle = std::wstring(title, title + strlen(title));

	//if (size == Vector2u{ (uint)(r.right - r.left), (uint)(r.bottom - r.top) })
		hwnd = CreateWindowEx(0, wc.lpszClassName, wtitle.c_str(), WS_POPUP, r.left, r.top, size.x, size.y, 0, 0, wc.hInstance, 0);
	//else
		//hwnd = CreateWindowEx(0, wc.lpszClassName, wtitle.c_str(), WS_OVERLAPPEDWINDOW, r.left, r.top, size.x, size.y, 0, 0, wc.hInstance, 0);
	if (!hwnd) {
		std::cout << "Window creation failed!" << std::endl;
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	hdc = GetDC(hwnd);

	return true;
}

void Window_Windows::Destroy() {
	if (hwnd) {
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
		hwnd = nullptr;
	}
}

void Window_Windows::PollEvents() {
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		switch (msg.message) {
			case WM_QUIT:
				open = false;
			break;

			case WM_MOUSEMOVE:
				//The following will lock the cursor to the render window:
				/*
				RECT rect;
				GetClientRect(hwnd, &rect);
				POINT ul = { rect.left, rect.top };
				POINT lr = { rect.right, rect.bottom };

				// Convert to screen space
				ClientToScreen(hwnd, &ul);
				ClientToScreen(hwnd, &lr);
				RECT clip_rect = { ul.x, ul.y, lr.x, lr.y };
				ClipCursor(&clip_rect);
				*/
				//The following will release the cursor from the render window:
				//ClipCursor(nullptr);

				ShowCursor(FALSE);
			break;

			case WM_MOUSELEAVE:
				ShowCursor(TRUE);
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window_Windows::Clear() {
	RECT rect;
	GetClientRect(hwnd, &rect);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdc, &rect, brush);
	DeleteObject(brush);
}