#include "Window_SDL.h"

Window_SDL::Window_SDL(const char* title, Vector2u size) {
	SDL_Init(SDL_INIT_VIDEO);
	
	int display_count = 0;
	SDL_DisplayID* displays = SDL_GetDisplays(&display_count);

	if (displays and display_count) {
		SDL_Rect bounds;
		if (SDL_GetDisplayBounds(displays[0], &bounds) == 0)
			screen_size = { bounds.w, bounds.h };
		else
			SDL_Log("Failed to get display bounds: %s", SDL_GetError());
	}
	SDL_free(displays);

	if (size == screen_size)
		window = SDL_CreateWindow(title, size.x, size.y, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	else
		window = SDL_CreateWindow(title, size.x, size.y, SDL_WINDOW_HIGH_PIXEL_DENSITY);

	win_size = size;

	if (window == NULL)
		std::cout << "Could not create window: " << SDL_GetError() << "\n";
}

void Window_SDL::PollEvents() {
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_EVENT_QUIT:
				open = false;
			break;
		}
	}
}