#include "Renderer.h"
#include "TileMap.h"
#include "../Core/Camera.h"
#include "../Core/Collision.h"
#include "../Core/Pathfinding.h"

Renderer::Renderer(SDL_Window* window, Camera* cam)
	: camera(cam) {
	
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	renderer = SDL_CreateRenderer(window, NULL);

	surface = SDL_CreateSurface(2560, 1440, SDL_PIXELFORMAT_RGBA8888);


	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void Renderer::DrawSheet(const Sprite& sheet, const Vector2i& pos) {
	const SDL_FRect dest = { pos.x, pos.y, sheet.info.sheet_size.x, sheet.info.sheet_size.y };

	SDL_RenderTexture(renderer, sheet.texture, NULL, &dest);
}

void Renderer::DrawSprite(const Sprite& spr) {
	//Cast to SDL
	const Sprite::Info* si = &spr.info;

	//Only draw sprites if they will be seen by the camera
	Vector2i sprite_pos = Round(si->pos.x - (si->spr_size.x * si->origin.x),
								si->pos.y - (si->spr_size.y * si->origin.y));
	if (Collision::AABB(camera->viewport, Rect(sprite_pos, si->spr_size))) {
		const SDL_FRect src = { si->curr_frame * si->frame_size.x,
									 si->sheet_row * si->frame_size.y,
									 si->frame_size.x, si->frame_size.y };


		const SDL_FRect dest = { sprite_pos.x - camera->viewport.x,
								sprite_pos.y - camera->viewport.y,
								si->spr_size.x,
								si->spr_size.y };

		//Set the tine
		SDL_SetTextureColorMod(spr.texture, si->tint.r * 255, si->tint.g * 255, si->tint.b * 255);
		SDL_SetTextureAlphaMod(spr.texture, si->tint.a * 255);

		SDL_RenderTexture(renderer, spr.texture, &src, &dest);
	}
}

void Renderer::DrawTilemap(TileMap& tmp) {

	SDL_Texture* ts_tex = nullptr;
	std::array<SDL_Vertex, 4> quad;
	std::array<int, 6> tile_inds = { 0, 1, 2, 0, 2, 3 };
	Vector2i tile_world_pos;

	for (auto& [ts_name, verts] : tmp.verts_by_tileset) {
		ts_tex = tmp.tilesets[ts_name];

		for (size_t i = 0; i < verts.size(); i += 4) {
			// Get tile vertices
			quad = {
				verts[i + 0],
				verts[i + 1],
				verts[i + 2],
				verts[i + 3]
			};

			// AABB cull in world space
			if (!Collision::AABB(camera->viewport, Rect({(int)quad[0].position.x, (int)quad[0].position.y}, TS)))
				continue;

			// Now convert to screen space
			for (auto& v : quad) {
				v.position.x = roundf(v.position.x - camera->viewport.x);
				v.position.y = roundf(v.position.y - camera->viewport.y);
			}

			// Draw visible tile
			SDL_RenderGeometry(renderer, ts_tex, quad.data(), 4, tile_inds.data(), 6);

		

		}
	}
}

void Renderer::DrawTxt(Text& txt) {
	Text::Info* ti = &txt.info;

	SDL_Color c = {
		static_cast<Uint8>(ti->color.r * 255),
		static_cast<Uint8>(ti->color.g * 255),
		static_cast<Uint8>(ti->color.b * 255),
		static_cast<Uint8>(ti->color.a * 255)
	};

	if (surface) SDL_DestroySurface(surface);
	if (!txt.font->GetFont()) {
		std::cout << "Font is null\n";
		return;
	}
	if (ti->str.length() == 0)
		return;
	surface = TTF_RenderText_Shaded_Wrapped(txt.font->GetFont(), ti->str.c_str(), ti->str.size(), c, SDL_Color{ 0,0,0,0 }, ti->max_width);

	if (!surface) {
		std::cout << "Failed to create text surface!\n";
		return;
	}
	ti->str_size = {(uint)surface->w, (uint)surface->h};

	if (texture) SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		std::cout << "Failed to create text texture!\n";
		return;
	}

	SDL_FRect src_rect = { 0, 0, surface->w, surface->h };
	SDL_FRect dest_rect = {
		round(ti->pos.x - (ti->str_size.x * ti->origin.x)) - camera->viewport.x,
		round(ti->pos.y - (ti->str_size.y * ti->origin.y)) - camera->viewport.y,
		static_cast<float>(surface->w),
		static_cast<float>(surface->h)
	};
	SDL_RenderTexture(renderer, texture, &src_rect, &dest_rect);

}

void Renderer::DrawNodeGrid(const Pathfinding& grid) {
	Rect node({0}, 4);
	for (int x = 0;x < grid.grid.size(); ++x) {
		for (int y = 0; y < grid.grid[x].size(); ++y) {
			if (grid.grid[x][y].walkable) {
				node.x = grid.grid[x][y].pos.x - 2;
				node.y = grid.grid[x][y].pos.y - 2;

				DrawRect(node, Color(0, 0, 1));
			}
		}
	}
}

void Renderer::DrawGrid(const Vector2i& start, const Vector2i& end, const uint& tile_size) {
	//Vertical Lines
	for (int i = start.x; i < end.x; i += tile_size)
		DrawLine(Line{ {i, start.y}, {i, end.y} }, Color(1, 0, 0));
	//Horizontal lines
	for (int i = start.y; i < end.y; i += tile_size)
		DrawLine(Line{ {start.x, i}, {end.x, i} }, Color(1, 0, 0));
}

void Renderer::DrawPath(std::queue<Vector2i> path, const Color& path_color) {

	vector<Vector2i> path_v;
	while (!path.empty()) {
		path_v.push_back(path.front());
		path.pop();
	}

	Rect point_box = { {0}, {4} };
	for (const auto& point : path_v) {
		point_box.x = point.x - 2; point_box.y = point.y - 2;
		DrawRect(point_box, path_color);
	}
}

void Renderer::DrawLine(const Line& line, const Color& color, const uint edge_w) {
	//SHOULD only draw if colliding with the camera but that requires implementing Collision::LineRect
	SDL_SetRenderDrawColor(renderer, color.r * 255, color.g * 255, color.b * 255, color.a * 255);

	SDL_RenderLine(renderer, line.x1 - camera->viewport.x, line.y1 - camera->viewport.y, line.x2 - camera->viewport.x, line.y2 - camera->viewport.y);
}

void Renderer::DrawCircle(const Circle& circle, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//SHOULD only draw if colliding with the camera but that requires implementing Collision::CircleRect
	Vector2f circle_pos = { (float)(circle.x - camera->viewport.x), (float)(circle.y - camera->viewport.y) };

	SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
	for (float r = circle.r; r > circle.r - edge_w; --r) {
		for (int w = -r; w < r; w++) {
			for (int h = -r; h < r; h++) {
				if (w * w + h * h <= r * r)
					SDL_RenderPoint(renderer, circle_pos.x + w, circle_pos.y + h);
			}
		}
	}

	float inner_r = circle.r - edge_w;
	SDL_SetRenderDrawColor(renderer, fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255);
	for (int w = -inner_r; w < inner_r; ++w) {
		for (int h = -inner_r; h < inner_r; ++h) {
			if (w * w + h * h <= inner_r * inner_r)
				SDL_RenderPoint(renderer, circle_pos.x + w, circle_pos.y + h);
		}
	}

}

void Renderer::DrawTri(const Tri& tri, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//SHOULD only draw if colliding with the camera but that requires implementing triangle collisions so fuck that
	Vector2f tri_pos1 = { (float)(tri.pos1.x - camera->viewport.x), (float)(tri.pos1.y - camera->viewport.y) };
	Vector2f tri_pos2 = { (float)(tri.pos2.x - camera->viewport.x), (float)(tri.pos2.y - camera->viewport.y) };
	Vector2f tri_pos3 = { (float)(tri.pos3.x - camera->viewport.x), (float)(tri.pos3.y - camera->viewport.y) };
	SDL_FColor f_color = { fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255 };

	//Draw the triangle
	SDL_Vertex verts[3];

	verts[0].position = { tri_pos1.x, tri_pos1.y };
	verts[0].color = f_color;

	verts[1].position = { tri_pos2.x, tri_pos2.y };
	verts[1].color = f_color;

	verts[2].position = { tri_pos3.x, tri_pos3.y };
	verts[2].color = f_color;

	SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);

	//Draw the edges
	SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
	SDL_RenderLine(renderer, tri_pos1.x, tri_pos1.y, tri_pos2.x, tri_pos2.y);
	SDL_RenderLine(renderer, tri_pos2.x, tri_pos2.y, tri_pos3.x, tri_pos3.y);
	SDL_RenderLine(renderer, tri_pos3.x, tri_pos3.y, tri_pos1.x, tri_pos1.y);
}

void Renderer::DrawRect(const Rect& rect, const Color& fill_color, const Color& stroke_color, const uint edge_w) {
	//Only draw if colliding with the camera
	if (Collision::AABB(rect, camera->viewport)) {
		Vector2f rect_pos = { (float)(rect.x - camera->viewport.x), (float)(rect.y - camera->viewport.y) };
		float w = rect.w;
		float h = rect.h;
		//Normalize w/h
		if (w < 0) {
			rect_pos.x += w;
			w = -w;
		}
		if (h < 0) {
			rect_pos.y += h;
			h = -h;
		}
		//Draw the fill
		SDL_SetRenderDrawColor(renderer, fill_color.r * 255, fill_color.g * 255, fill_color.b * 255, fill_color.a * 255);
		SDL_FRect sdl_rect = { rect_pos.x, rect_pos.y, w, h };
		SDL_RenderFillRect(renderer, &sdl_rect);

		//Draw the edges
		SDL_SetRenderDrawColor(renderer, stroke_color.r * 255, stroke_color.g * 255, stroke_color.b * 255, stroke_color.a * 255);
		//Top
		SDL_FRect top = { rect_pos.x, rect_pos.y, w, edge_w };
		SDL_RenderFillRect(renderer, &top);
		//Bottom
		SDL_FRect bot = { rect_pos.x, (rect_pos.y + h) - edge_w, w, edge_w };
		SDL_RenderFillRect(renderer, &bot);
		//Left
		SDL_FRect left = { rect_pos.x, rect_pos.y + edge_w, edge_w, h - (edge_w * 2) };
		SDL_RenderFillRect(renderer, &left);
		//Right
		SDL_FRect right = { rect_pos.x + w - edge_w, rect_pos.y + edge_w, edge_w, h - edge_w * 2 };
		SDL_RenderFillRect(renderer, &right);
	}
}