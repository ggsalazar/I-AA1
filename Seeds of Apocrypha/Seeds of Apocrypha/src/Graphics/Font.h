#pragma once
#include <filesystem>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

class Font {
public:
	Font() = default;
	Font(const std::string& path, uint size) {
		std::string path_ttf = "assets/Fonts/" + path + ".ttf";
		font = TTF_OpenFont(path_ttf.c_str(), size);

		if (!font)
			std::cout << "Could not load font from file: " << path << "! SDL Error: " << SDL_GetError() << "\n";
	}
	~Font() {
		//TTF_DestroyFont(font);
	}

	inline TTF_Font* GetFont() const { if (font) return font; return nullptr; }

private:
	TTF_Font* font = nullptr;
};