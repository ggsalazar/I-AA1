#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_surface.h>
#include "TileMap.h"

int TS = 16;
int METER = 32;

bool TileMap::Load(SDL_Renderer* renderer, const string& json_file) {
	//Unload the old tilemap, if there was one
	Unload();

	//Load json file
	std::ifstream file("assets/Maps/" + json_file + ".json");
	if (!file.is_open()) {
		cerr << "Failed to open " << json_file << " map!/n";
		return false;
	}

	file >> tilemap_data;

	//Map size in tiles/pixels
	map_size_t = { tilemap_data["width"], tilemap_data["height"] };
	map_size_p = map_size_t * TS;

	//Resize tile_data to be the size of the map
	tile_data.resize(map_size_t.x, vector<Tile>(map_size_t.y));

	//Load all of our needed textures
	tilesets["Default"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Default.png");
	if (json_file == "Tutton") {
		tilesets["Grass"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Grass.png");
		tilesets["Stone"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Stone.png");
		tilesets["Water"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Water.png");
		tilesets["Wood"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Wood.png");
	}
	else if (json_file == "Debug") {
		tilesets["Grass"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Grass.png");
		tilesets["Stone"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Stone.png");
		tilesets["Water"] = IMG_LoadTexture(renderer, "assets/Sprites/Environments/TileSets/Water.png");
	}
	for (auto& [name, tex] : tilesets)
		SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);


	string ts_name;
	SDL_Texture* ts_tex;
	Vector2f tile_pos, tex_size;
	Vector2u tile_uv, vert_uv;
	SDL_Vertex vert[4];
	int t_p_r, base;
	uint global_tile_id, local_tile_id, firstgid;
	uint col = 0, row = 0;

	//Populate tile_data
	for (const auto& layer : tilemap_data["layers"]) {
		for (col = 0; col < map_size_t.x; ++col) {
			for (row = 0; row < map_size_t.y; ++row) {
				//Get current tile ID number
				global_tile_id = layer["data"][col + row * map_size_t.x];

				//Skip empty tiles
				if (!global_tile_id) continue;

				//Get the name of the tileset
				for (const auto& tileset : tilemap_data["tilesets"]) {
					firstgid = tileset["firstgid"];
					if (global_tile_id >= firstgid) {
						ts_name = tileset["source"].get<string>();
						//Ensure a tileset was found
						if (ts_name.empty()) {
							cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset!\n";
							continue;
						}

						ts_name.erase(ts_name.length() - 4); //Remove ".tsx"
						local_tile_id = global_tile_id - firstgid;
					}
					else break;
				}

				//Add the data of the current tile to the tile_data 2D vector
				//Add the name of the tileset as a terrain type
				if (ts_name == "Stone" or ts_name == "Wood" or ts_name == "Grass")
					tile_data[col][row].terrain = Terrain::Normal;
				else if (ts_name == "Water")
					tile_data[col][row].terrain = Terrain::Water;

				//Get the elevation from the name of the layer (e.g. "Elev 0")
				string elev = layer["name"]; //Have to remove it from the JSON first
				tile_data[col][row].elevation = stof(elev.substr(5));

				//Populate vertex buffer
				tile_pos = { (float)(col * TS), (float)(row * TS) };

				//Set vertex positions
				vert[0].position = { tile_pos.x, tile_pos.y };
				vert[1].position = { tile_pos.x + TS, tile_pos.y };
				vert[2].position = { tile_pos.x + TS, tile_pos.y + TS };
				vert[3].position = { tile_pos.x, tile_pos.y + TS };

				//Get the texture
				if (tilesets.find(ts_name) != tilesets.end())
					ts_tex = tilesets[ts_name];
				else {
					cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset! Assigning Default\n";
					ts_tex = tilesets["Default"];
				}
				SDL_GetTextureSize(ts_tex, &tex_size.x, &tex_size.y);
				t_p_r = tex_size.x / BASE_TS;
				tile_uv = { local_tile_id % t_p_r, local_tile_id / t_p_r };
				vert_uv = tile_uv * BASE_TS;

				vert[0].tex_coord = {vert_uv.x / tex_size.x, vert_uv.y / tex_size.y};
				vert[1].tex_coord = { (vert_uv.x + BASE_TS) / tex_size.x, vert_uv.y / tex_size.y };
				vert[2].tex_coord = { (vert_uv.x + BASE_TS) / tex_size.x, (vert_uv.y + BASE_TS) / tex_size.y };
				vert[3].tex_coord = { vert_uv.x / tex_size.x, (vert_uv.y + BASE_TS) / tex_size.y };

				for (int i = 0; i < 4; ++i)
					vert[i].color = { 1.f, 1.f, 1.f, 1.f };

				auto& verts = verts_by_tileset[ts_name];
				auto& inds = indices_by_tileset[ts_name];
				base = verts.size();

				verts.insert(verts.end(), std::begin(vert), std::end(vert));
				inds.insert(inds.end(), { base, base + 1, base + 2, base, base + 2, base + 3 });
			}
		}
	}

	map_loaded = true;
	return true;
}

void TileMap::Unload() {
	for (const auto& [ts_name, ts_tex] : tilesets)
		SDL_DestroyTexture(ts_tex);


	verts_by_tileset.clear();
	indices_by_tileset.clear();
	tilesets.clear();
	tile_data.clear();
}
