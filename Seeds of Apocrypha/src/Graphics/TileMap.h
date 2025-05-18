#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Core/Enums.h"
#include "../Core/Vector2.h"

using json = nlohmann::json;
using std::vector, std::cerr, std::cout, std::unordered_map, std::string;

//Tile Size
constexpr uint TS = 16;
constexpr uint METER = 32;

struct Tile {
	Terrains terrain = Terrains::NORMAL;
	float elevation = 0.f;
};

class TileMap {
	friend class Renderer;
	friend class Pathfinding;
public:

	//Tiling
	bool Load(SDL_Renderer* renderer, const string& json_file);

	void Unload();

	inline bool Loaded() const { return map_loaded; }

	inline Vector2u GetMapSizeTiles() const { return map_size_t; }
	inline Vector2u GetMapSizePixels() const { return map_size_p; }

	inline Tile GetTileData(Vector2i tile_pos) { return tile_data[floor(tile_pos.x)][floor(tile_pos.y)]; }

private:
	unordered_map<string, vector<SDL_Vertex>> verts_by_tileset;
	unordered_map<string, vector<int>> indices_by_tileset;
	unordered_map<string, SDL_Texture*> tilesets;

	vector<vector<Tile>> tile_data;
	json tilemap_data;

	Vector2u map_size_t = { 0, 0 };
	Vector2u map_size_p = { 0, 0 };
	bool map_loaded = false;
};