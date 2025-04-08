#include <Windows.h>
#include "Renderer_D2D.h"

namespace Engine {

void Renderer_D2D::Init(void* window_handle) {
	//Create Factory
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

	//Get client rect for window size
	RECT r;
	GetClientRect((HWND)window_handle, &r);

	//Create render target
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_props = D2D1::HwndRenderTargetProperties((HWND)window_handle, D2D1::SizeU(r.right - r.left, r.bottom - r.top));

	factory->CreateHwndRenderTarget(props, hwnd_props, &render_target);

	render_target->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 0.f), &brush);
}

void Renderer_D2D::Shutdown() {
	if (render_target) render_target->Release();
	if (factory) factory->Release();
	if (brush) brush->Release();
}

void Renderer_D2D::DrawLine(const Line& line, const Color& color) {
	brush->SetColor(D2D1::ColorF(color.r, color.g, color.b)); brush->SetOpacity(color.a);
	render_target->DrawLine(D2D1::Point2F(line.pos1.x, line.pos1.y), D2D1::Point2F(line.pos2.x, line.pos2.y), brush, line.w);
}

void Renderer_D2D::DrawCircle(const Circle& circle, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//Edge
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawEllipse(D2D1_ELLIPSE({ D2D1::Point2F(circle.pos.x, circle.pos.y), circle.r, circle.r }), brush, edge_w);

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	render_target->FillEllipse(D2D1_ELLIPSE({ D2D1::Point2F(circle.pos.x, circle.pos.y), circle.r, circle.r }), brush);
}

void Renderer_D2D::DrawTri(const Tri& tri, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//Construct the triangle geometry
	ID2D1PathGeometry* geometry = nullptr;
	ID2D1GeometrySink* sink = nullptr;

	factory->CreatePathGeometry(&geometry);
	geometry->Open(&sink);
	sink->BeginFigure(D2D1::Point2F(tri.pos1.x, tri.pos1.y), D2D1_FIGURE_BEGIN_FILLED);
	D2D1_POINT_2F points[] = { D2D1::Point2F(tri.pos2.x, tri.pos2.y), D2D1::Point2F(tri.pos3.x, tri.pos3.y) };
	sink->AddLines(points, ARRAYSIZE(points));
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();

	//Edge
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawGeometry(geometry, brush, edge_w);

	//Fill
	//brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	brush->SetColor(D2D1::ColorF(1, 1, 1)); brush->SetOpacity(fill_color.a);
	render_target->FillGeometry(geometry, brush);



	geometry->Release();
	sink->Release();
}

void Renderer_D2D::DrawRect(const Rect& rect, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//Edge
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawRectangle(D2D1::RectF(rect.pos.x, rect.pos.y, rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), brush, edge_w);

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	render_target->FillRectangle(D2D1::RectF(rect.pos.x, rect.pos.y, rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), brush);
}

}