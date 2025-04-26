#include <Windows.h>
#include "Renderer_D2D.h"

Renderer_D2D::Renderer_D2D(void* window_handle)	{
	// Get window size
	HWND hwnd = static_cast<HWND>(window_handle);
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// Create D3D11 device and swap chain
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 2;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	D3D_FEATURE_LEVEL featureLevel;
	ComPtr<ID3D11Texture2D> back_buffer;

	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		nullptr, 0, D3D11_SDK_VERSION, &scd, &swap_chain, &d3d_device,
		&featureLevel, &d3d_context);

	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));

	// Create D2D factory
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&factory));

	// Get DXGI device from D3D device
	ComPtr<IDXGIDevice> dxgi_device;
	d3d_device.As(&dxgi_device);

	// Create D2D device and device context
	factory->CreateDevice(dxgi_device.Get(), &d2d_device);
	d2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &device_context);

	// Create D2D render target bitmap from back buffer
	ComPtr<IDXGISurface> dxgi_surface;
	back_buffer.As(&dxgi_surface);

	D2D1_BITMAP_PROPERTIES1 bitmap_props = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED));

	ComPtr<ID2D1Bitmap1> d2d_bitmap;
	device_context->CreateBitmapFromDxgiSurface(dxgi_surface.Get(), &bitmap_props, &d2d_bitmap);

	device_context->SetTarget(d2d_bitmap.Get());

	//Create write factory and brush
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(dwrite_factory.GetAddressOf()));

	device_context->CreateSolidColorBrush(D2D1::ColorF(0.f, 0.f, 0.f, 0.f), &brush);
	device_context->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	device_context->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);

	//Get the scale factor
	scale_factor = GetDpiForWindow(hwnd) / 96.f;
}

void Renderer_D2D::DrawSheet(const Sprite& sheet, const Vector2i& pos) {
	const Sprite_D2D* sheetd2d = static_cast<const Sprite_D2D*>(&sheet);

	D2D1_POINT_2F true_pos = D2D1::Point2F(pos.x / scale_factor, pos.y / scale_factor);

	device_context->DrawBitmap(sheetd2d->GetBitmap(), D2D1::RectF(true_pos.x, true_pos.y,
							  true_pos.x + sheet.GetSheetSize().x/scale_factor, true_pos.y + sheet.GetSheetSize().y/scale_factor));
}

void Renderer_D2D::DrawSprite(Sprite& spr) {
	
	//Cast to D2D
	Sprite_D2D* sprd2d = static_cast<Sprite_D2D*>(&spr);
	Sprite::Info* si = &sprd2d->info;

	//Set draw location; let D2D handle float precision, otherwise will get pixel distortion
	D2D1_POINT_2F draw_pos = D2D1::Point2F(
		(si->pos.x - (si->spr_size.x * si->origin.x)) / scale_factor,
		(si->pos.y - (si->spr_size.y * si->origin.y)) / scale_factor
	);

	//Set the frame to draw
	D2D1_RECT_F frame_rect = D2D1::RectF(
		si->curr_frame * si->frame_size.x,
		si->sheet_row * si->frame_size.y,
		si->curr_frame * si->frame_size.x + si->frame_size.x,
		si->sheet_row * si->frame_size.y + si->frame_size.y
	);
	
	//Set the tint, if any
	ComPtr<ID2D1Effect> tint_effect;
	HRESULT hr = device_context->CreateEffect(CLSID_D2D1ColorMatrix, &tint_effect);
	tint_effect->SetInput(0, sprd2d->GetBitmap());
	D2D1_MATRIX_5X4_F tint = CreateTintMatrix(si->tint);
	tint_effect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, tint);

	//Set the scale
	D2D1_MATRIX_3X2_F transform = D2D1::Matrix3x2F::Translation(draw_pos.x, draw_pos.y) *
		D2D1::Matrix3x2F::Scale(
			si->scale.x, si->scale.y,
			draw_pos);

	device_context->SetTransform(transform);
	device_context->DrawImage(
		tint_effect.Get(),	//Image to draw (includes post-tint bitmap)
		nullptr,			//Draw destination
		&frame_rect,		//Portion of bitmap to draw
		D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1_COMPOSITE_MODE_SOURCE_OVER
	);

}

void Renderer_D2D::DrawTxt(Text& txt) {
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
		ti->max_width / scale_factor, FLT_MAX / scale_factor,
		&text_layout
	);
	if (FAILED(hr)) {
		cout << "Failed to create Text Layout in Renderer_D2D.cpp" << endl;
		return;
	}
	DWRITE_TEXT_METRICS metrics;
	text_layout->GetMetrics(&metrics);
	ti->str_size = { (uint)round(metrics.width), (uint)round(metrics.height) };
	//Adjust position for scale factor
	//D2D1_POINT_2F posd2d = D2D1::Point2F((ti->pos.x - (ti->str_size.x * ti->origin.x)) / scale_factor,
	//									 (ti->pos.y - (ti->str_size.y * ti->origin.y)) / scale_factor);
	D2D1_POINT_2F posd2d = D2D1::Point2F((ti->pos.x - (ti->str_size.x * ti->origin.x)),
										 (ti->pos.y - (ti->str_size.y * ti->origin.y)));

	//Set the scale
	device_context->DrawTextLayout(posd2d, text_layout.Get(), brush.Get());
}

void Renderer_D2D::DrawLine(const Line& line, const Color& color, const uint edge_w) {
	brush->SetColor(D2D1::ColorF(color.r, color.g, color.b)); brush->SetOpacity(color.a);

	device_context->DrawLine(D2D1::Point2F(line.pos1.x / scale_factor, line.pos1.y / scale_factor),
		D2D1::Point2F(line.pos2.x / scale_factor, line.pos2.y / scale_factor),
		brush.Get(), edge_w);
}

void Renderer_D2D::DrawCircle(const Circle& circle, const Color& stroke_color, const Color& fill_color, const uint edge_w) {
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

void Renderer_D2D::DrawTri(const Tri& tri, const Color& stroke_color, const Color& fill_color, const uint edge_w) {
	//Construct the triangle geometry
	ComPtr<ID2D1PathGeometry> geometry;
	ComPtr<ID2D1GeometrySink> sink;

	factory->CreatePathGeometry(&geometry);
	geometry->Open(&sink);
	sink->BeginFigure(D2D1::Point2F(tri.pos1.x / scale_factor, tri.pos1.y / scale_factor), D2D1_FIGURE_BEGIN_FILLED);
	D2D1_POINT_2F points[] = { D2D1::Point2F(tri.pos2.x / scale_factor, tri.pos2.y / scale_factor), D2D1::Point2F(tri.pos3.x / scale_factor, tri.pos3.y / scale_factor) };
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

void Renderer_D2D::DrawRect(const Rect& rect, const Color& stroke_color, const Color& fill_color, const uint edge_w) {

	D2D1_POINT_2F posd2d = D2D1::Point2F(rect.pos.x / scale_factor, rect.pos.y / scale_factor);

	//Fill
	brush->SetColor(D2D1::ColorF(fill_color.r, fill_color.g, fill_color.b)); brush->SetOpacity(fill_color.a);
	device_context->FillRectangle(D2D1::RectF(posd2d.x, posd2d.y, (posd2d.x + rect.size.x / scale_factor), (posd2d.y + rect.size.y / scale_factor)), brush.Get());

	//Stroke
	brush->SetColor(D2D1::ColorF(stroke_color.r, stroke_color.g, stroke_color.b)); brush->SetOpacity(stroke_color.a);
	device_context->DrawRectangle(D2D1::RectF(posd2d.x, posd2d.y, (posd2d.x + rect.size.x / scale_factor), (posd2d.y + rect.size.y / scale_factor)), brush.Get(), edge_w);
}