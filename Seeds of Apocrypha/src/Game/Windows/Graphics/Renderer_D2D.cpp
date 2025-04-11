#include <Windows.h>
#include "Renderer_D2D.h"

namespace Engine {

void Renderer_D2D::Init(void* window_handle) {
	//Create Factory
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&factory));

	//Get client rect for window size
	RECT r;
	GetClientRect((HWND)window_handle, &r);

	//Create render target
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_props = D2D1::HwndRenderTargetProperties((HWND)window_handle, D2D1::SizeU(r.right - r.left, r.bottom - r.top));

	factory->CreateHwndRenderTarget(props, hwnd_props, &render_target);

	render_target->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 0.f), &brush);
}

void Renderer_D2D::DrawLine(const Line& line, const Color& color) {
	brush->SetColor(D2D1::ColorF(color.r, color.g, color.b)); brush->SetOpacity(color.a);
	render_target->DrawLine(D2D1::Point2F(line.pos1.x, line.pos1.y), D2D1::Point2F(line.pos2.x, line.pos2.y), brush.Get(), line.w);
}

void Renderer_D2D::DrawCircle(const Circle& circle, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//Edge
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawEllipse(D2D1_ELLIPSE({ D2D1::Point2F(circle.pos.x, circle.pos.y), circle.r, circle.r }), brush.Get(), edge_w);

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	render_target->FillEllipse(D2D1_ELLIPSE({ D2D1::Point2F(circle.pos.x, circle.pos.y), circle.r, circle.r }), brush.Get());
}

void Renderer_D2D::DrawTri(const Tri& tri, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//Construct the triangle geometry
	ComPtr<ID2D1PathGeometry> geometry;
	ComPtr<ID2D1GeometrySink> sink;

	factory->CreatePathGeometry(&geometry);
	geometry->Open(&sink);
	sink->BeginFigure(D2D1::Point2F(tri.pos1.x, tri.pos1.y), D2D1_FIGURE_BEGIN_FILLED);
	D2D1_POINT_2F points[] = { D2D1::Point2F(tri.pos2.x, tri.pos2.y), D2D1::Point2F(tri.pos3.x, tri.pos3.y) };
	sink->AddLines(points, ARRAYSIZE(points));
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();

	//Edge
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawGeometry(geometry.Get(), brush.Get(), edge_w);

	//Fill
	brush->SetColor(D2D1::ColorF(1, 1, 1)); brush->SetOpacity(fill_color.a);
	render_target->FillGeometry(geometry.Get(), brush.Get());
}

void Renderer_D2D::DrawRect(const Rect& rect, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//Edge
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawRectangle(D2D1::RectF(rect.pos.x, rect.pos.y, rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), brush.Get(), edge_w);

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	render_target->FillRectangle(D2D1::RectF(rect.pos.x, rect.pos.y, rect.pos.x + rect.size.x, rect.pos.y + rect.size.y), brush.Get());
}

void Renderer_D2D::DrawSheet(const Spritesheet& sheet, const Vector2u& pos) {
	const Spritesheet_D2D* ssd2d = static_cast<const Spritesheet_D2D*>(&sheet);

	render_target->DrawBitmap(ssd2d->GetBitmap(), D2D1::RectF(pos.x, pos.y, pos.x + sheet.GetSize().x, pos.y + sheet.GetSize().y));
}

void Renderer_D2D::DrawSprite(const Sprite& spr) {
	//Cast to D2D
	const Sprite_D2D* sprd2d = static_cast<const Sprite_D2D*>(&spr);
	Spritesheet_D2D* ssd2d = static_cast<Spritesheet_D2D*>(sprd2d->sheet.get());

	const Sprite::Info* si = &sprd2d->info;

	FLOAT dpi_x, dpi_y;
	render_target->GetDpi(&dpi_x, &dpi_y);
	float scale_factor = dpi_x / 96.f; //dpi_x and y should be equal

	//Set draw location
	float pos_x = (si->pos.x - (si->spr_size.x * si->origin.x)) / scale_factor,
		  pos_y = (si->pos.y - (si->spr_size.y * si->origin.y)) / scale_factor;
	D2D1_RECT_F pos_rect = D2D1::RectF(pos_x, pos_y, pos_x+si->spr_size.x, pos_y+si->spr_size.y);

	//Set the frame to draw
	D2D1_RECT_F frame_rect = D2D1::RectF(si->curr_frame * si->frame_size.x,
										 si->sheet_row * si->frame_size.y,
										 si->curr_frame * si->frame_size.x + si->frame_size.x,
										 si->sheet_row * si->frame_size.y + si->frame_size.y);

	//Draw the current frame
	render_target->DrawBitmap(ssd2d->GetBitmap(),								//Bitmap to draw from
							  pos_rect,											//Destination rect
							  si->color.a,										//Opacity
							  D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,  //Interpolation mode
							  frame_rect);										//Portion of bitmap to draw
}

}