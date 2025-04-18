#include <Windows.h>
#include "Renderer_D2D.h"

namespace Engine {
Renderer_D2D::Renderer_D2D(void* window_handle)	{
	//Create Factory
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&factory));

	//Create Write Factory
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(dwrite_factory.GetAddressOf()));

	//Get client rect for window size
	RECT r;
	GetClientRect((HWND)window_handle, &r);

	//Create render target
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_props = D2D1::HwndRenderTargetProperties((HWND)window_handle, D2D1::SizeU(r.right - r.left, r.bottom - r.top));

	factory->CreateHwndRenderTarget(props, hwnd_props, &render_target);

	render_target->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 0.f), &brush);

	//Set scale factor
	FLOAT dpi_x, dpi_y;
	render_target->GetDpi(&dpi_x, &dpi_y);
	scale_factor = dpi_x / 96.f; //dpi_x and y should be equal
}

void Renderer_D2D::DrawSheet(const Sprite& sheet, const Vector2u& pos) {
	const Sprite_D2D* sheetd2d = static_cast<const Sprite_D2D*>(&sheet);

	//Set draw location
	Vector2u new_pos = (round(pos.x / scale_factor), round(pos.y / scale_factor));

	render_target->DrawBitmap(sheetd2d->GetBitmap(), D2D1::RectF(new_pos.x, new_pos.y, new_pos.x + round(sheet.GetSheetSize().x/scale_factor), new_pos.y + round(sheet.GetSheetSize().y/scale_factor)));
}

void Renderer_D2D::DrawSprite(const Sprite& spr) {
	//Cast to D2D
	const Sprite_D2D* sprd2d = static_cast<const Sprite_D2D*>(&spr);

	const Sprite::Info* si = &sprd2d->info;

	//Set draw location
	Vector2f pos = { round((si->pos.x - (si->spr_size.x * si->origin.x)) / scale_factor),
					round((si->pos.y - (si->spr_size.y * si->origin.y)) / scale_factor) };
	D2D1_RECT_F pos_rect = D2D1::RectF(pos.x, pos.y, pos.x + round(si->spr_size.x/scale_factor), pos.y + round(si->spr_size.y/scale_factor));

	//Set the frame to draw
	D2D1_RECT_F frame_rect = D2D1::RectF(
		si->curr_frame * si->frame_size.x,
		si->sheet_row * si->frame_size.y,
		si->curr_frame * si->frame_size.x + si->frame_size.x,
		si->sheet_row * si->frame_size.y + si->frame_size.y);

	//Draw the current frame
	render_target->DrawBitmap(sprd2d->GetBitmap(),			//Bitmap to draw from
		pos_rect,											//Destination rect
		si->color.a,										//Opacity
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,    //Interpolation mode
		frame_rect);										//Portion of bitmap to draw
}

void Renderer_D2D::DrawTxt(Text& txt) {
	//Cast to D2D
	Text::Info* ti = &txt.info;
	Font_D2D* fntd2d = static_cast<Font_D2D*>(ti->font);

	//Create text format
	IDWriteTextFormat* text_format = fntd2d->GetFormat(ti->char_size);

	//Set the brush
	Color* c = &ti->color;
	brush->SetColor(D2D1::ColorF(c->r, c->g, c->b, c->a));

	ComPtr<IDWriteTextLayout> text_layout;

	int size_needed = MultiByteToWideChar(CP_UTF8, 0, ti->str.c_str(), -1, nullptr, 0);
	std::wstring text(size_needed - 1, 0);
	MultiByteToWideChar(CP_UTF8, 0, ti->str.c_str(), -1, &text[0], size_needed);

	//Automatically handles string wrapping
	HRESULT hr = dwrite_factory->CreateTextLayout(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		text_format,
		ti->max_width/scale_factor, FLT_MAX,
		&text_layout
	);
	if (FAILED(hr)) {
		std::cout << "Failed to create Text Layout in Renderer_D2D.cpp" << std::endl;
		return;
	}
	DWRITE_TEXT_METRICS metrics;
	text_layout->GetMetrics(&metrics);
	ti->str_size = { (uint)round(metrics.width), (uint)round(metrics.height) };

	//std::cout << "String size: " << ti->str_size << std::endl;

	//Adjust position for scale factor
	D2D1_POINT_2F posd2d = D2D1::Point2F(round((ti->pos.x - (ti->str_size.x * ti->origin.x)) / scale_factor),
										round((ti->pos.y - (ti->str_size.y * ti->origin.y)) / scale_factor));
	render_target->DrawTextLayout(posd2d, text_layout.Get(), brush.Get());
}

void Renderer_D2D::DrawLine(const Line& line, const Color& color, const uint edge_w) {
	brush->SetColor(D2D1::ColorF(color.r, color.g, color.b)); brush->SetOpacity(color.a);

	render_target->DrawLine(D2D1::Point2F(round(line.pos1.x / scale_factor),
		round(line.pos1.y / scale_factor)),
		D2D1::Point2F(round(line.pos2.x / scale_factor), round(line.pos2.y / scale_factor)),
		brush.Get(), edge_w);
}

void Renderer_D2D::DrawCircle(const Circle& circle, const Color& stroke_color, const Color& fill_color, const uint edge_w) {
	//Adjusted position
	D2D1_POINT_2F posd2d = D2D1::Point2F(round(circle.pos.x / scale_factor), round(circle.pos.y / scale_factor));
	

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	render_target->FillEllipse(D2D1_ELLIPSE({ posd2d, round(circle.r / scale_factor), round(circle.r / scale_factor) }), brush.Get());

	//Stroke (we want it drawn on top of the fill)
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawEllipse(D2D1_ELLIPSE({ posd2d, round(circle.r / scale_factor), round(circle.r / scale_factor) }), brush.Get(), edge_w);
}

void Renderer_D2D::DrawTri(const Tri& tri, const Color& stroke_color, const Color& fill_color, const uint edge_w) {
	//Construct the triangle geometry
	ComPtr<ID2D1PathGeometry> geometry;
	ComPtr<ID2D1GeometrySink> sink;

	factory->CreatePathGeometry(&geometry);
	geometry->Open(&sink);
	sink->BeginFigure(D2D1::Point2F(round(tri.pos1.x / scale_factor), round(tri.pos1.y / scale_factor)), D2D1_FIGURE_BEGIN_FILLED);
	D2D1_POINT_2F points[] = { D2D1::Point2F(round(tri.pos2.x / scale_factor), round(tri.pos2.y / scale_factor)), D2D1::Point2F(round(tri.pos3.x / scale_factor), round(tri.pos3.y / scale_factor)) };
	sink->AddLines(points, ARRAYSIZE(points));
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();

	//Fill
	brush->SetColor(D2D1::ColorF(1, 1, 1)); brush->SetOpacity(fill_color.a);
	render_target->FillGeometry(geometry.Get(), brush.Get());

	//Stroke (we want it drawn on top of the fill)
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawGeometry(geometry.Get(), brush.Get(), edge_w);
}

void Renderer_D2D::DrawRect(const Rect& rect, const Color& stroke_color, const Color& fill_color, const uint edge_w) {
	//Adjusted position
	D2D1_POINT_2F posd2d = D2D1::Point2F(round(rect.pos.x / scale_factor), round(rect.pos.y / scale_factor));
	
	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	render_target->FillRectangle(D2D1::RectF(posd2d.x, posd2d.y, posd2d.x + round(rect.size.x / scale_factor), posd2d.y + round(rect.size.y / scale_factor)), brush.Get());

	//Stroke
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	render_target->DrawRectangle(D2D1::RectF(posd2d.x, posd2d.y, posd2d.x + round(rect.size.x / scale_factor), posd2d.y + round(rect.size.y / scale_factor)), brush.Get(), edge_w);
}
}