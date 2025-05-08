#pragma once
#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "../Core/Geometry.h"
#include "Sprite.h" //(Aliases.h)
#include "Text.h" //(Font.h)

class Renderer {
public:
	Renderer(SDL_Window* window);
	~Renderer() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroySurface(surface);
		SDL_DestroyTexture(texture);
	}

	//Functionality
	SDL_Renderer* GetRenderer() const { return renderer; }
	void BeginFrame() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}
	void EndFrame() {
		SDL_RenderPresent(renderer);
	}

	//Sprites
	void DrawSheet(const Sprite& sheet, const Vector2i& pos = { 0 }); //Helpful for debugging/seeing entire sheet
	void DrawSprite(Sprite& spr);
	//Text
	void DrawTxt(Text& txt);

	//Drawing shapes
	void DrawLine(const Line& line, const Color& color = Color(1), const uint edge_w = 2);
	void DrawCircle(const Circle& circle, const Color& stroke_color = Color(1), Color fill_color = Color(0, 0, 0, 0), const uint edge_w = 2);
	void DrawTri(const Tri& tri, const Color& stroke_color = Color(1), Color fill_color = Color(0, 0, 0, 0), const uint edge_w = 2);
	void DrawRect(const Rect& rect, const Color& stroke_color = Color(1), Color fill_color = Color(0, 0, 0, 0), const uint edge_w = 2);
	
private:
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Texture* texture;
};