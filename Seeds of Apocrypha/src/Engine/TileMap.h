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

	struct Node {
		sf::Vector2u pos;
		bool walkable;
		//TO-DO: flyable, swimmable
		float cost = 1.f; //1 for normal ground, .5 for rough terrain, 0 for slightly dangerous, -1 for moderately dangerous, -2 for highly dangerous
		float g = 0.f, h = 0.f, f = 0.f; //For A* calculations
		Node* parent = nullptr;
	};

	//Tiling
	bool load(const string& json_file) {
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
		map_size_p = sf::Vector2f(map_size_t) * static_cast<float>(TS);

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
			for (unsigned int row = 0; row < map_size_t.x; ++row) {
				for (unsigned int col = 0; col < map_size_t.y; ++col) {
					//Get current tile ID number
					const unsigned int global_tile_id = layer["data"][row + col * map_size_t.x];

					//Can't skip empty tiles bc of node grid
					//if (!global_tile_id) continue;

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
						tile_data[row][col].terrain = Terrains::NORMAL;
					else if (ts_name == "Water")
						tile_data[row][col].terrain = Terrains::WATER;

					//Get the elevation from the name of the layer (e.g. "Elev 0")
					string elev = layer["name"]; //Have to remove it from the JSON first
					tile_data[row][col].elevation = stof(elev.substr(5));


					sf::Vector2u tileset_size = ts_tex->getSize();

					//Find the tile's position in the tileset texture
					const int tiles_per_row = tileset_size.x >> 5; //Equivalent to tileset_size.x / 32
					const int tu = local_tile_id & (tiles_per_row - 1); //Equivalent to local_tile_id % tiles_per_row
					const int tv = local_tile_id >> 5; //Equivalent to local_tile_id / tiles_per_row

					//Assign vertices for rendering
					sf::Vertex* tri = &m_vertices_by_tileset[ts_name][(row + col * map_size_t.x) * 6];

					tri[0].position = sf::Vector2f(round(row * TS), round(col * TS));
					tri[1].position = sf::Vector2f(round((row + 1) * TS), round(col * TS));
					tri[2].position = sf::Vector2f(round(row * TS), round((col + 1) * TS));
					tri[3].position = sf::Vector2f(round(row * TS), round((col + 1) * TS));
					tri[4].position = sf::Vector2f(round((row + 1) * TS), round(col * TS));
					tri[5].position = sf::Vector2f(round((row + 1) * TS), round((col + 1) * TS));

					tri[0].texCoords = sf::Vector2f(tu * TS, tv * TS);
					tri[1].texCoords = sf::Vector2f((tu + 1) * TS, tv * TS);
					tri[2].texCoords = sf::Vector2f(tu * TS, (tv + 1) * TS);
					tri[3].texCoords = sf::Vector2f(tu * TS, (tv + 1) * TS);
					tri[4].texCoords = sf::Vector2f((tu + 1) * TS, tv * TS);
					tri[5].texCoords = sf::Vector2f((tu + 1) * TS, (tv + 1) * TS);
				}
			}
		}

		//Populate node grid
		unsigned int x = map_size_t.x * 3 - (map_size_t.x - 1);
		unsigned int y = map_size_t.y * 3 - (map_size_t.y - 1);
		for (unsigned int row = 0; row < x; ++row) {
			for (unsigned int col = 0; col < y; ++col) {
				/*
				sf::Vector2u pos;
				bool walkable;
				//TO-DO: flyable, swimmable
				float cost = 1.f; //1 for normal ground, .5 for rough terrain, 0 for slightly dangerous, -1 for moderately dangerous, -2 for highly dangerous
				float g = 0.f, h = 0.f, f = 0.f; //For A* calculations
				Node* parent = nullptr;
				*/
				if (!row or !col or (row == x - 1) or (col == y - 1)) continue;

				Node node;
				sf::Vector2u node_pos = { row * 16, col * 16 };
				bool node_walk = true;
				float node_cost = 1.f;
				float node_g = 0.f, node_h = 0.f, node_f = 0.f;
				Node* node_par = nullptr;
				//First node - very top left of the map
				if (!row and !col) {
					node_walk = tile_data[row-1][col-1].terrain != Terrains::WATER or
								tile_data[row + 1][col - 1].terrain != Terrains::WATER or
					node = { {row*16, col*16}, 
							  tile_data[0][0].terrain != Terrains::WATER,
							  1.f };
				}
				//The rest of the nodes
				else {
					node = { {row * 16,col * 16},
							tile_data[][].terrain != Terrains::WATER,
							1.f,
							0.f, 0.f, 0.f,
							&grid.back().back() };
				}


				grid[row][col] = node;
			}
		}


		map_loaded = true;
		return true;
	}

	bool Loaded() const { return map_loaded; }

	sf::Vector2u GetMapSizeTiles() const { return map_size_t; }
	sf::Vector2f GetMapSizePixels() const { return map_size_p; }

	Tile GetTileData(sf::Vector2f tile_pos) { return tile_data[floor(tile_pos.x)][floor(tile_pos.y)]; }

	//Pathfinding
	float Heuristic(const sf::Vector2i& a, sf::Vector2i& b) {
		uint dx = abs(a.x - b.x);
		uint dy = abs(a.y - b.y);
		return (dx + dy) + (sqrt(2) - 2) * min(dx, dy);
	}

	vector<sf::Vector2i> FindPath(const sf::Vector2i& start, const sf::Vector2i& goal) {

		//No path found
		return {};
	}

private:
	unordered_map<string, sf::VertexArray> m_vertices_by_tileset;
	unordered_map<string, sf::Texture> m_tilesets;

	sf::Vector2u map_size_t = { 0, 0 };
	sf::Vector2f map_size_p = { 0.f, 0.f };

	vector<vector<Node>> grid;

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