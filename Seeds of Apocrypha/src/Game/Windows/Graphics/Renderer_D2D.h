#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>
#include "../../../Engine/Graphics/Renderer.h"
#include "Sprite_D2D.h" //Includes Spritesheet_D2D.h

using Microsoft::WRL::ComPtr;

namespace Engine {

class Renderer_D2D : public Renderer {
public:
	Renderer_D2D() {}
	~Renderer_D2D() override {}

	//Functionality
	void Init(void* window_handle) override;
	inline void BeginFrame() override { render_target->BeginDraw(); Clear(0, 0, 0, 1); }
	inline void EndFrame() override { render_target->EndDraw(); }
	inline void Clear(float r, float g, float b, float a) override { render_target->Clear(D2D1::ColorF(r, g, b, a)); }
	inline ID2D1HwndRenderTarget* GetRT() const { return render_target.Get(); }
	
	//Shapes
	void DrawLine(const Line& line, const Color& color) override;
	void DrawCircle(const Circle& circle, const Color& fill_color, const Color& stroke_color, const uint edge_w = 2) override;
	void DrawTri(const Tri& tri, const Color& fill_color, const Color& stroke_color, const uint edge_w = 2) override;
	void DrawRect(const Rect& rect, const Color& fill_color, const Color& stroke_color, const uint edge_w = 2) override;
	
	//Sprites
	void DrawSheet(const Spritesheet& sprsht, const Vector2u& pos = {0, 0}) override;
	void DrawSprite(const Sprite& spr) override;

private:
	ComPtr<ID2D1Factory> factory;
	ComPtr<ID2D1HwndRenderTarget> render_target;

	//Brush for drawing shapes
	ComPtr<ID2D1SolidColorBrush> brush;
};

}