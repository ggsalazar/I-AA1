#pragma once
#include <SDL.h>
#include <SDL_render.h>
#include "../../../Engine/Graphics/Renderer.h" //Includes Sprite.h and Text.h
#include "Font_SDL.h" //Needs to be included or DrawText complains at me
#include "Sprite_SDL.h"

#undef DrawText

using namespace Engine;

class Renderer_SDL : public Renderer {
public:
	Renderer_SDL(SDL_Window* window);
	~Renderer_SDL() override { SDL_DestroyRenderer(renderer); }

	SDL_Renderer* GetRenderer() const { return renderer; }

	//Functionality
	inline void BeginFrame() override {
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}
	inline void EndFrame() override {
		SDL_RenderPresent(renderer);
	}

	//Sprites
	void DrawSheet(const Sprite& sheet, const Vector2i& pos = { 0, 0 }) override;
	void DrawSprite(Sprite& spr) override;
	//Text
	void DrawText(Text& txt) override;

	//Shapes
	void DrawLine(const Line& line, const Color& color = Color(1), const uint edge_w = 2) override;
	void DrawCircle(const Circle& circle, const Color& stroke_color = Color(1), Color fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	void DrawTri(const Tri& tri, const Color& stroke_color = Color(1), Color fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	void DrawRect(const Rect& rect, const Color& stroke_color = Color(1), Color fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	
private:
	SDL_Renderer* renderer;
	SDL_Surface* surface;
};