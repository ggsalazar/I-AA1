#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <wrl.h>
#include "../../../Engine/Graphics/Renderer.h" //Includes Sprite.h and Text.h
#include "Fonts/Font_D2D.h" //Needs to be included or DrawTxt complains at me
#include "Sprite_D2D.h"

using Microsoft::WRL::ComPtr;

namespace Engine {

class Renderer_D2D : public Renderer {
public:
	Renderer_D2D(void* window_handle);
	~Renderer_D2D() override {}

	//Functionality
	inline void BeginFrame() override { render_target->BeginDraw(); Clear(0, 0, 0, 1); }
	inline void EndFrame() override { render_target->EndDraw(); }
	inline void Clear(float r, float g, float b, float a) override { render_target->Clear(D2D1::ColorF(r, g, b, a)); }
	//Pointers 'n shit
	inline ID2D1HwndRenderTarget* GetRT() const { return render_target.Get(); }
	inline IDWriteFactory* GetDWriteFactory() const { return dwrite_factory.Get(); }

	//Sprites
	void DrawSheet(const Sprite& sheet, const Vector2u& pos = { 0, 0 }) override;
	void DrawSprite(const Sprite& spr) override;
	//Text
	void DrawTxt(Text& txt) override;

	//Shapes
	void DrawLine(const Line& line, const Color& color) override;
	void DrawCircle(const Circle& circle, const Color& fill_color, const Color& stroke_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	void DrawTri(const Tri& tri, const Color& fill_color, const Color& stroke_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	void DrawRect(const Rect& rect, const Color& fill_color, const Color& stroke_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	
private:
	float scale_factor;
	ComPtr<ID2D1Factory> factory;
	ComPtr<ID2D1HwndRenderTarget> render_target;

	//Write factory for text
	ComPtr<IDWriteFactory> dwrite_factory;

	//Brush for drawing shapes
	ComPtr<ID2D1SolidColorBrush> brush;

};

}