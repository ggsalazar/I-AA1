#pragma once
#include "../Core/Geometry.h"
#include "Sprite.h" //Just trust that this needs to be here
#include "Text.h"

namespace Engine {
class Renderer {
public:
	virtual ~Renderer() {}

	//Functionality
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
	virtual void Clear(float r, float g, float b, float a) = 0;

	//Sprites
	virtual void DrawSheet(const Sprite& sheet, const Vector2i& pos = { 0, 0 }) = 0; //Helpful for debugging/seeing entire sheet
	virtual void DrawSprite(Sprite& spr) = 0;
	//Text
	virtual void DrawTxt(Text& txt) = 0;

	//Drawing shapes
	virtual void DrawLine(const Line& line, const Color& color = Color(1), const uint edge_w = 2) = 0;
	virtual void DrawCircle(const Circle& circle, const Color& stroke_color = Color(1), const Color& fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) = 0;
	virtual void DrawTri(const Tri& tri, const Color& stroke_color = Color(1), const Color& fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) = 0;
	virtual void DrawRect(const Rect& rect, const Color& stroke_color = Color(1), const Color& fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) = 0;
	
};
}