#pragma once
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1effects.h>
#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <dwrite.h>
#include <wrl.h>
#include "../../../Engine/Graphics/Renderer.h" //Includes Sprite.h and Text.h
#include "Fonts/Font_D2D.h" //Needs to be included or DrawTxt complains at me
#include "Sprite_D2D.h"

using Microsoft::WRL::ComPtr;
using namespace Engine;

class Renderer_D2D : public Renderer {
public:
	Renderer_D2D(void* window_handle);
	~Renderer_D2D() override {}

	//Functionality
	inline void BeginFrame() override { 
		device_context->BeginDraw();
		//Reset the transform matrix
		device_context->SetTransform(D2D1::Matrix3x2F::Identity());
		device_context->Clear(D2D1::ColorF(0, 0, 0, 1));
	}
	inline void EndFrame() override {
		device_context->EndDraw();
		swap_chain->Present(1, 0); //Vsync on
	}
	inline void Clear(float r, float g, float b, float a) override {}
	//Pointers 'n shit
	inline ID2D1DeviceContext* GetDC() const { return device_context.Get(); }
	inline IDWriteFactory* GetDWriteFactory() const { return dwrite_factory.Get(); }

	//Tint Matrix for dynamic sprite tinting
	D2D1_MATRIX_5X4_F CreateTintMatrix(Color& color) {
		return D2D1::Matrix5x4F(
			color.r, 0.0f, 0.0f, 0.0f, //R' = R * r
			0.0f, color.g, 0.0f, 0.0f, //G' = G * g
			0.0f, 0.0f, color.b, 0.0f, //B' = B * b
			0.0f, 0.0f, 0.0f, color.a, //A' = A * a
			0.0f, 0.0f, 0.0f, 0.0f); //No offset
	}

	//Sprites
	void DrawSheet(const Sprite& sheet, const Vector2i& pos = { 0, 0 }) override;
	void DrawSprite(Sprite& spr) override;
	//Text
	void DrawTxt(Text& txt) override;

	//Shapes
	void DrawLine(const Line& line, const Color& color = Color(1), const uint edge_w = 2) override;
	void DrawCircle(const Circle& circle, const Color& stroke_color = Color(1), const Color& fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	void DrawTri(const Tri& tri, const Color& stroke_color = Color(1), const Color& fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	void DrawRect(const Rect& rect, const Color& stroke_color = Color(1), const Color& fill_color = Color(0, 0, 0, 0), const uint edge_w = 2) override;
	
private:
	float scale_factor;
	ComPtr<ID2D1Factory1> factory;
	ComPtr<IDWriteFactory> dwrite_factory;
	ComPtr<IWICBitmap> bitmap;
	ComPtr<ID2D1Device> d2d_device;
	ComPtr<ID2D1DeviceContext> device_context;
	//D3D Pointers
	ComPtr<ID3D11Device> d3d_device;
	ComPtr<ID3D11DeviceContext> d3d_context;
	ComPtr<IDXGISwapChain> swap_chain;
	

	//Brush for drawing shapes
	ComPtr<ID2D1SolidColorBrush> brush;

};