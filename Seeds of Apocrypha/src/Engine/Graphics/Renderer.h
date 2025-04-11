#pragma once
#include "../Core/Geometry.h"
#include "Sprite.h" //Includes Spritesheet.h

namespace Engine {
class Renderer {
public:
	virtual ~Renderer() {}

	//Functionality
	virtual void Init(void* window_handle) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Clear(float r, float g, float b, float a) = 0;
	//virtual void Shutdown() = 0;
	
	//Drawing shapes
	virtual void DrawLine(const Line& line, const Color& color) = 0;
	virtual void DrawCircle(const Circle& circle, const Color& fill_color, const Color& line_color = Color(0, 0, 0, 0), const uint edge_w = 2) = 0;
	virtual void DrawTri(const Tri& tri, const Color& fill_color, const Color& line_color = Color(0, 0, 0, 0), const uint edge_w = 2) = 0;
	virtual void DrawRect(const Rect& rect, const Color& fill_color, const Color& line_color = Color(0, 0, 0, 0), const uint edge_w = 2) = 0;
	
	//Sprites
	virtual void DrawSheet(const Spritesheet& sprsht, const Vector2u& pos = {0, 0}) = 0; //Helpful for debugging/seeing entire sheet
	virtual void DrawSprite(const Sprite& spr) = 0;
};
}