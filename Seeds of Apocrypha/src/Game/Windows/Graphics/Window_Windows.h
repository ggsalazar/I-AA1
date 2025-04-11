#pragma once
#include <Windows.h>
#include <vector>
#include "../../../Engine/Graphics/Window.h"

namespace Engine {

class Window_Windows : public Window {
public:
	bool open = true;

	Window_Windows(uint m) : monitor(m) {}
	~Window_Windows() { Destroy(); }

	bool Create(const char* title, Vector2u size = {0, 0}) override;
	void Destroy() override;

	void PollEvents() override;

	void Clear() override;
	void Display() override {} //Why do I have this?

	inline Vector2u GetSize() const override { return win_size; }
	HWND GetHandle() const { return hwnd; }

private:
	HWND hwnd = nullptr;
	HDC hdc = nullptr;
	uint monitor;
	Vector2u win_size;

	struct MonitorInfo {
		HMONITOR handle;
		RECT rect;
	};

	std::vector<MonitorInfo> EnumerateMonitors() {
		std::vector<MonitorInfo> monitors;

		EnumDisplayMonitors(nullptr, nullptr,
			[](HMONITOR hMon, HDC, LPRECT, LPARAM data) -> BOOL {
				MONITORINFO info = {};
				info.cbSize = sizeof(info);
				if (GetMonitorInfo(hMon, &info)) {
					auto& out = *reinterpret_cast<std::vector<MonitorInfo>*>(data);
					out.push_back({ hMon, info.rcMonitor });
				}
				return TRUE;
			},
			reinterpret_cast<LPARAM>(&monitors)
		);

		return monitors;
	}

};

}