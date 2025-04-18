#include <windowsx.h>
#include "Window_Windows.h"

namespace Engine {

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

	//Make system aware of DPI scaling (no manual adjustment needed)
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
	win_size = size;

	//Convert the title from a const char* to a wide string
	std::wstring wtitle = std::wstring(title, title + strlen(title));

	hwnd = CreateWindowEx(0, wc.lpszClassName, wtitle.c_str(), WS_OVERLAPPEDWINDOW, r.left, r.top, size.x, size.y, 0, 0, wc.hInstance, 0);
	if (!hwnd) {
		std::cout << "Window creation failed!" << std::endl;
		return false;
	}

	//Cannot create the window borderless for some fucking reason that I can't figure out, so have to change it after the fact
	if (size == Vector2u{ (uint)(r.right - r.left), (uint)(r.bottom - r.top) })
		SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);

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
		if (msg.message == WM_QUIT) open = false;

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
}