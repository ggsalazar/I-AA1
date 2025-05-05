#pragma once
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <vector>
#include "../../../Engine/Graphics/Window.h"

class Window_SDL : public Engine::Window {
public:
	bool open = true;

	Window_SDL(const char* title, Vector2u size = { 0, 0 });
	~Window_SDL() {}

	void PollEvents() override;

	inline Vector2u ScreenSize() override { return screen_size; }
	inline SDL_Window* GetWin() const { return window; }
private:
	SDL_Window* window;
	SDL_Event event;
	Vector2u screen_size;
};