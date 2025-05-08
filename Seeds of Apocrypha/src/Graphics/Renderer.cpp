#include "Renderer.h"

Renderer::Renderer(SDL_Window* window) {

	SDL_Init(SDL_INIT_VIDEO);

	renderer = SDL_CreateRenderer(window, NULL);

	surface = SDL_CreateSurface(2560, 1440, SDL_PIXELFORMAT_RGBA8888);


	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	if (!SDL_SetRenderVSync(renderer, 1))
		std::cout << "Failed to set VSync: " << SDL_GetError() << "\n";
}

void Renderer::DrawSheet(const Sprite& sheet, const Vector2i& pos) {
	const SDL_FRect dest_rect = { pos.x, pos.y, sheet.info.sheet_size.x, sheet.info.sheet_size.y };

	SDL_RenderTexture(renderer, sheet.texture, NULL, &dest_rect);
}

void Renderer::DrawSprite(Sprite& spr) {
	//Cast to SDL
	Sprite::Info* si = &spr.info;
	
	const SDL_FRect src_rect = {si->curr_frame * si->frame_size.x,
								 si->sheet_row * si->frame_size.y,
								 si->curr_frame * si->frame_size.x + si->frame_size.x,
								 si->sheet_row * si->frame_size.y + si->frame_size.y };
								 

	const SDL_FRect dest_rect = { round(si->pos.x - (si->spr_size.x * si->origin.x)),
								  round(si->pos.y - (si->spr_size.y * si->origin.y)),
								  si->spr_size.x,
								  si->spr_size.y};

	//Set the tine
	SDL_SetTextureColorMod(spr.texture, si->tint.r * 255, si->tint.g * 255, si->tint.b * 255);
	SDL_SetTextureAlphaMod(spr.texture, si->tint.a * 255);

	SDL_RenderTexture(renderer, spr.texture, &src_rect, &dest_rect);
	
}

void Renderer::DrawTxt(Text& txt) {
	//Cast to SDL
	Text::Info* ti = &txt.info;
	
	SDL_Color c = {
		static_cast<Uint8>(ti->color.r * 255),
		static_cast<Uint8>(ti->color.g * 255),
		static_cast<Uint8>(ti->color.b * 255),
		static_cast<Uint8>(ti->color.a * 255)
	};

	surface = TTF_RenderText_Shaded_Wrapped(txt.font->GetFont(), ti->str.c_str(), ti->str.size(), c, SDL_Color{ 0,0,0,0 }, ti->max_width);
	if (!surface) {
		std::cout << "Failed to create text surface!\n";
		return;
	}
	ti->str_size = { (uint)surface->w, (uint)surface->h };

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cout << "Failed to create text texture!\n";
		return;
	}

	SDL_FRect src_rect = { 0, 0, surface->w, surface->h };
	SDL_FRect dest_rect = {
		round(ti->pos.x - (ti->str_size.x * ti->origin.x)),
		round(ti->pos.y - (ti->str_size.y * ti->origin.y)),
		static_cast<float>(surface->w),
		static_cast<float>(surface->h)
	};
	SDL_RenderTexture(renderer, texture, &src_rect, &dest_rect);
	
}

void Renderer::DrawLine(const Line& line, const Color& color, const uint edge_w) {
	SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

	SDL_RenderLine(renderer, line.pos1.x, line.pos1.y, line.pos2.x, line.pos2.y);
}

void Renderer::DrawCircle(const Circle& circle, const Color& stroke_color, Color fill_color, const uint edge_w) {

	SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
	for (float r = circle.r; r > circle.r - edge_w; --r) {
		for (int w = -r; w < r; w++) {
			for (int h = -r; h < r; h++) {
				if (w * w + h * h <= r * r)
					SDL_RenderPoint(renderer, circle.x + w, circle.y + h);
			}
		}
	}

	float inner_r = circle.r - edge_w;
	SDL_SetRenderDrawColor(renderer, fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255);
	for (int w = -inner_r; w < inner_r; ++w) {
		for (int h = -inner_r; h < inner_r; ++h) {
			if (w * w + h * h <= inner_r * inner_r)
				SDL_RenderPoint(renderer, circle.x + w, circle.y + h);
		}
	}
	
}

void Renderer::DrawTri(const Tri& tri, const Color& stroke_color, Color fill_color, const uint edge_w) {
	//Draw the triangle
	SDL_Vertex verts[3];

	verts[0].position.x = tri.pos1.x;
	verts[0].position.y = tri.pos1.y;
	verts[0].color.r = fill_color.r * 255;
	verts[0].color.g = fill_color.g * 255;
	verts[0].color.b = fill_color.b * 255;
	verts[0].color.a = fill_color.a * 255;

	verts[1].position.x = tri.pos2.x;
	verts[1].position.y = tri.pos2.y;
	verts[1].color.r = fill_color.r * 255;
	verts[1].color.g = fill_color.g * 255;
	verts[1].color.b = fill_color.b * 255;
	verts[1].color.a = fill_color.a * 255;

	verts[2].position.x = tri.pos3.x;
	verts[2].position.y = tri.pos3.y;
	verts[2].color.r = fill_color.r * 255;
	verts[2].color.g = fill_color.g * 255;
	verts[2].color.b = fill_color.b * 255;
	verts[2].color.a = fill_color.a * 255;

	SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);

	//Draw the edges
	SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
	SDL_RenderLine(renderer, tri.pos1.x, tri.pos1.y, tri.pos2.x, tri.pos2.y);
	SDL_RenderLine(renderer, tri.pos2.x, tri.pos2.y, tri.pos3.x, tri.pos3.y);
	SDL_RenderLine(renderer, tri.pos3.x, tri.pos3.y, tri.pos1.x, tri.pos1.y);

}

void Renderer::DrawRect(const Rect& rect, const Color& stroke_color, Color fill_color, const uint edge_w) {
	//Draw the fill
	SDL_SetRenderDrawColor(renderer, fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255);
	SDL_FRect sdl_rect = { rect.x, rect.y, rect.w, rect.h };
	SDL_RenderFillRect(renderer, &sdl_rect);

	//Have to draw the edges manually, unfortunately
	SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
	//Top
	SDL_FRect top = { rect.x, rect.y, rect.w, edge_w };
	SDL_RenderFillRect(renderer, &top);
	//Bottom
	SDL_FRect bot = { rect.x, rect.y + rect.h-edge_w, rect.w, edge_w };
	SDL_RenderFillRect(renderer, &bot);
	//Left
	SDL_FRect left = { rect.x, rect.y, edge_w, rect.h };
	SDL_RenderFillRect(renderer, &left);
	//Right
	SDL_FRect right = { rect.x + rect.w-edge_w, rect.y, edge_w, rect.h };
	SDL_RenderFillRect(renderer, &right);
}