#include <Windows.h>
#include "Renderer_SDL.h"

Renderer_SDL::Renderer_SDL(SDL_Window* window)	{

	renderer = SDL_CreateRenderer(window, "Renderer 1");

	if (!SDL_SetRenderVSync(renderer, 1))
		cout << "Failed to set VSync: " << SDL_GetError() << "\n";

}

void Renderer_SDL::DrawSheet(const Sprite& sheet, const Vector2i& pos) {
	const Sprite_SDL* sheetsdl = static_cast<const Sprite_SDL*>(&sheet);

	const SDL_FRect dest_rect = { pos.x, pos.y, sheetsdl->info.sheet_size.x, sheetsdl->info.sheet_size.y };

	SDL_RenderTexture(renderer, sheetsdl->texture, NULL, &dest_rect);
}

void Renderer_SDL::DrawSprite(Sprite& spr) {
	//Cast to SDL
	Sprite_SDL* sprsdl = static_cast<Sprite_SDL*>(&spr);
	Sprite::Info* si = &sprsdl->info;

	const SDL_FRect src_rect = { si->curr_frame * si->frame_size.x,
								 si->sheet_row * si->frame_size.y,
								 si->curr_frame * si->frame_size.x + si->frame_size.x,
								 si->sheet_row * si->frame_size.y + si->frame_size.y };

	const SDL_FRect dest_rect = { si->pos.x - (si->spr_size.x * si->origin.x),
							 	  si->pos.y - (si->spr_size.y * si->origin.y) };

	//Set the tint (TO-DO)

	SDL_RenderTexture(renderer, sprsdl->texture, &src_rect, &dest_rect);
}

void Renderer_SDL::DrawText(Text& txt) {
	//Cast to D2D
	Text::Info* ti = &txt.info;
	Font_D2D* fntd2d = static_cast<Font_D2D*>(txt.font);

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
		ti->max_width, FLT_MAX,
		&text_layout
	);
	if (FAILED(hr)) {
		cout << "Failed to create Text Layout in Renderer_SDL.cpp" << endl;
		return;
	}
	DWRITE_TEXT_METRICS metrics;
	text_layout->GetMetrics(&metrics);
	ti->str_size = { (uint)round(metrics.width), (uint)round(metrics.height) };
	//Adjust position for scale factor
	//D2D1_POINT_2F posd2d = D2D1::Point2F((ti->pos.x - (ti->str_size.x * ti->origin.x)),
	//									 (ti->pos.y - (ti->str_size.y * ti->origin.y)));
	D2D1_POINT_2F posd2d = D2D1::Point2F((ti->pos.x - (ti->str_size.x * ti->origin.x)),
										 (ti->pos.y - (ti->str_size.y * ti->origin.y)));

	//Set the scale
	device_context->DrawTextLayout(posd2d, text_layout.Get(), brush.Get());
}

void Renderer_SDL::DrawLine(const Line& line, const Color& color, const uint edge_w) {
	brush->SetColor(D2D1::ColorF(color.r, color.g, color.b)); brush->SetOpacity(color.a);

	device_context->DrawLine(D2D1::Point2F(line.pos1.x, line.pos1.y),
		D2D1::Point2F(line.pos2.x, line.pos2.y),
		brush.Get(), edge_w);
}

void Renderer_SDL::DrawCircle(const Circle& circle, const Color& stroke_color, Color fill_color, const uint edge_w) {
	if (stroke_color.r != 1 or stroke_color.g != 1 or stroke_color.b != 1)
		fill_color = stroke_color;
	
	//Adjusted position
	D2D1_POINT_2F posd2d = D2D1::Point2F(circle.pos.x, circle.pos.y);
	FLOAT true_rad = circle.r;

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	device_context->FillEllipse(D2D1_ELLIPSE({ posd2d, true_rad, true_rad }), brush.Get());

	//Stroke (we want it drawn on top of the fill)
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	device_context->DrawEllipse(D2D1_ELLIPSE({ posd2d, true_rad, true_rad }), brush.Get(), edge_w);
}

void Renderer_SDL::DrawTri(const Tri& tri, const Color& stroke_color, Color fill_color, const uint edge_w) {
	if (stroke_color.r != 1 or stroke_color.g != 1 or stroke_color.b != 1)
		fill_color = stroke_color;

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

	//Fill
	brush->SetColor(D2D1::ColorF(1, 1, 1)); brush->SetOpacity(fill_color.a);
	device_context->FillGeometry(geometry.Get(), brush.Get());

	//Stroke (we want it drawn on top of the fill)
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	device_context->DrawGeometry(geometry.Get(), brush.Get(), edge_w);
}

void Renderer_SDL::DrawRect(const Rect& rect, const Color& stroke_color, Color fill_color, const uint edge_w) {
	if (stroke_color.r != 1 or stroke_color.g != 1 or stroke_color.b != 1)
		fill_color = stroke_color;

	D2D1_POINT_2F posd2d = D2D1::Point2F(rect.pos.x, rect.pos.y);

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	device_context->FillRectangle(D2D1::RectF(posd2d.x, posd2d.y, (posd2d.x + rect.size.x), (posd2d.y + rect.size.y)), brush.Get());

	//Stroke
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	device_context->DrawRectangle(D2D1::RectF(posd2d.x, posd2d.y, (posd2d.x + rect.size.x), (posd2d.y + rect.size.y)), brush.Get(), edge_w);
}