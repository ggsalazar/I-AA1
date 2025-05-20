#pragma once
#include <queue>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "Sprite.h" //(Aliases.h, Geometry.h)
#include "Text.h" //(Font.h)

class Camera;
class TileMap;
class Pathfinding;

class Renderer {
public:
	Renderer(SDL_Window* window, Camera* cam);
	~Renderer() {
		if (renderer) SDL_DestroyRenderer(renderer);
		if (surface) SDL_DestroySurface(surface);
		if (texture) SDL_DestroyTexture(texture);
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
	void DrawSprite(const Sprite& spr);
	void DrawTilemap(TileMap& tmp);
	//Text
	void DrawTxt(Text& txt);

	void DrawNodeGrid(const Pathfinding& grid);
	void DrawGrid(const Vector2i& start = { 0, 0 }, const Vector2i& end = { 1920, 1080 }, const uint& tile_size = 32);

	void DrawPath(std::queue<Vector2i> path);

	//Drawing shapes
	void DrawLine(const Line& line, const Color& color = Color(1), const uint edge_w = 2);
	void DrawCircle(const Circle& circle, const Color& fill_color = Color(1), const Color& stroke_color = Color(0,0,0,0), const uint edge_w = 2);
	void DrawTri(const Tri& tri, const Color& fill_color = Color(1), const Color& stroke_color = Color(0,0,0,0), const uint edge_w = 2);
	void DrawRect(const Rect& rect, const Color& fill_color = Color(1), const Color& stroke_color = Color(0,0,0,0), const uint edge_w = 2);

private:
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;
	Camera* camera = nullptr;
};