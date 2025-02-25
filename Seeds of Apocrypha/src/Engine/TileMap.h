#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "Utils/Enums.h"

using json = nlohmann::json;
using namespace std;

struct Tile {
	Terrains terrain;
	float elevation;
};

class TileMap : public sf::Drawable, public sf::Transformable {
public:

	bool load(const string& json_file, sf::Vector2u tile_size = {32, 32}) {
		//Load json file
		ifstream file(json_file);
		if (!file.is_open()) {
			cerr << "Failed to open " << json_file << " map!" << endl;
			return false;
		}

		json tilemap_data;
		file >> tilemap_data;

		//Map size in tiles/pixels
		map_size_t = { tilemap_data["width"], tilemap_data["height"] };
		map_size_p = sf::Vector2f(map_size_t) * 32.f;

		//Resize tile_data to be the size of the map
		tile_data.resize(map_size_t.x, vector<Tile>(map_size_t.y));

		//Resize m_v_b_ts only as many times as necessary, instead of for literally every tile
		string ts_name;
		for (const auto& ts : tilemap_data["tilesets"]) {
			ts_name = ts["source"].get<string>();
			ts_name.erase(ts_name.length() - 4); //Remove ".tsx"
			m_vertices_by_tileset[ts_name].setPrimitiveType(sf::PrimitiveType::Triangles);
			m_vertices_by_tileset[ts_name].resize(map_size_t.x * map_size_t.y * 6);
		}

		//Populate vertex array
		for (const auto& layer : tilemap_data["layers"]) {
			for (unsigned int x = 0; x < map_size_t.x; ++x) {
				for (unsigned int y = 0; y < map_size_t.y; ++y) {
					//Get current tile ID number
					const unsigned int global_tile_id = layer["data"][x + y * map_size_t.x];

					//Skip empty tiles
					if (!global_tile_id) continue;

					//Identify the correct tileset for this tile
					sf::Texture* ts_tex = nullptr;
					unsigned int local_tile_id = global_tile_id;

					for (const auto& tileset : tilemap_data["tilesets"]) {
						const unsigned int firstgid = tileset["firstgid"];
						if (global_tile_id >= firstgid) {
							ts_name = tileset["source"].get<string>();
							ts_name.erase(ts_name.length() - 4); //Remove ".tsx"
							local_tile_id = global_tile_id - firstgid;
						}
						else break;
					}
					
					//Ensure a tileset was found
					if (ts_name.empty()) {
						cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset!" << endl;
						continue;
					}
					//Avoid unnecessarily reloading the same texture multiple times
					if (m_tilesets.find(ts_name) == m_tilesets.end()) {
						if (!m_tilesets[ts_name].loadFromFile("assets/Sprites/Environments/TileSets/" + ts_name + ".png")) {
							cerr << "Failed to load tileset: " << ts_name << endl;
							continue;
						}
					}
					m_tilesets[ts_name].setSmooth(false);
					ts_tex = &m_tilesets[ts_name];
					if (!ts_tex) {
						cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset!" << endl;
						continue;
					}

					//Add the data of the current tile to the tile_data 2D vector
					//Add the name of the tileset as a terrain type
					if (ts_name == "Stone" or ts_name == "Wood" or ts_name == "Grass")
						tile_data[x][y].terrain = Terrains::NORMAL;
					else if (ts_name == "Water")
						tile_data[x][y].terrain = Terrains::WATER;
					
					//Get the elevation from the name of the layer (e.g. "Elev 0")
					string elev = layer["name"]; //Have to remove it from the JSON first
					tile_data[x][y].elevation = stof(elev.substr(5));


					sf::Vector2u tileset_size = ts_tex->getSize();

					//Find the tile's position in the tileset texture
					const int tiles_per_row = tileset_size.x >> 5; //Equivalent to tileset_size.x / 32
					const int tu = local_tile_id & (tiles_per_row-1); //Equivalent to local_tile_id % tiles_per_row
					const int tv = local_tile_id >> 5; //Equivalent to local_tile_id / tiles_per_row

					//Assign vertices for rendering
					sf::Vertex* tri = &m_vertices_by_tileset[ts_name][(x+y*map_size_t.x) * 6];

					tri[0].position = sf::Vector2f(round(x * tile_size.x), round(y * tile_size.y));
					tri[1].position = sf::Vector2f(round((x + 1) * tile_size.x), round(y * tile_size.y));
					tri[2].position = sf::Vector2f(round(x * tile_size.x), round((y + 1) * tile_size.y));
					tri[3].position = sf::Vector2f(round(x * tile_size.x), round((y + 1) * tile_size.y));
					tri[4].position = sf::Vector2f(round((x + 1) * tile_size.x), round(y * tile_size.y));
					tri[5].position = sf::Vector2f(round((x + 1) * tile_size.x), round((y + 1) * tile_size.y));

					tri[0].texCoords = sf::Vector2f(tu * tile_size.x, tv * tile_size.y);
					tri[1].texCoords = sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
					tri[2].texCoords = sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
					tri[3].texCoords = sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
					tri[4].texCoords = sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
					tri[5].texCoords = sf::Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
				}
			}
		}

		map_loaded = true;
		return true;
	}

	bool Loaded() const { return map_loaded; }

	sf::Vector2u GetMapSizeTiles() const { return map_size_t; }
	sf::Vector2f GetMapSizePixels() const { return map_size_p; }

	Tile GetTileData(sf::Vector2f tile_pos) { return tile_data[floor(tile_pos.x)][floor(tile_pos.y)]; }

private:
	unordered_map<string, sf::VertexArray> m_vertices_by_tileset;
	unordered_map<string, sf::Texture> m_tilesets;

	sf::Vector2u map_size_t = { 0, 0 };
	sf::Vector2f map_size_p = { 0.f, 0.f };

	vector<vector<Tile>> tile_data;
	bool map_loaded = false;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
		//Apply the transform
		states.transform *= getTransform();

		//Iterate over each tileset and draw only the vertices using that texture
		for (const auto& [ts_name, ts_tex] : m_tilesets) {
			states.texture = &ts_tex;

			//Find which vertices belong to this tileset
			target.draw(m_vertices_by_tileset.at(ts_name), states);
		}
	}
};