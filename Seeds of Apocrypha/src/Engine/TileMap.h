#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "Utils/Enums.h"
#include "Utils/Macros.h"

using json = nlohmann::json;
using namespace std;

struct Tile {
	Terrains terrain = Terrains::NORMAL;
	float elevation = 0.f;
};


class TileMap : public sf::Drawable, public sf::Transformable {
public:

	struct Node {
		sf::Vector2i pos = { 0, 0 };
		bool debug = false; //Activate if in the closed list
		bool walkable = false;
		//TO-DO: flyable, swimmable
		uint cost = 1; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
		float g = 0.f, h = 0.f, f = 0.f; //For A* calculations
		Node* parent = nullptr;
	};

	//Tiling
	bool load(const string& json_file) {
		//Load json file
		ifstream file("assets/Sprites/Environments/TileMaps/" + json_file + ".json");
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


		//Load all of our needed textures only once
		m_tilesets["Default"].loadFromFile("assets/Sprites/Environments/TileSets/Default.png");
		if (json_file == "Tutton") {
			m_tilesets["Grass"].loadFromFile("assets/Sprites/Environments/TileSets/Grass.png");
			m_tilesets["Stone"].loadFromFile("assets/Sprites/Environments/TileSets/Stone.png");
			m_tilesets["Water"].loadFromFile("assets/Sprites/Environments/TileSets/Water.png");
			m_tilesets["Wood"].loadFromFile("assets/Sprites/Environments/TileSets/Wood.png");
		}
		else if (json_file == "Debug_Room") {
			m_tilesets["Grass"].loadFromFile("assets/Sprites/Environments/TileSets/Grass.png");
			m_tilesets["Stone"].loadFromFile("assets/Sprites/Environments/TileSets/Stone.png");
			m_tilesets["Water"].loadFromFile("assets/Sprites/Environments/TileSets/Water.png");
		}
		for (auto& [name, ts] : m_tilesets)
			ts.setSmooth(false);


		//Populate vertex array
		for (const auto& layer : tilemap_data["layers"]) {
			for (uint row = 0; row < map_size_t.x; ++row) {
				for (uint col = 0; col < map_size_t.y; ++col) {
					//Get current tile ID number
					const uint global_tile_id = layer["data"][row + col * map_size_t.x];

					//Skip empty tiles
					if (!global_tile_id) continue;

					//Identify the correct tileset for this tile
					sf::Texture* ts_tex = nullptr;
					uint local_tile_id = global_tile_id;

					for (const auto& tileset : tilemap_data["tilesets"]) {
						const uint firstgid = tileset["firstgid"];
						if (global_tile_id >= firstgid) {
							ts_name = tileset["source"].get<string>();
							//Ensure a tileset was found
							if (ts_name.empty()) {
								cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset!" << endl;
								continue;
							}
							//Remove ".tsx"
							ts_name.erase(ts_name.length() - 4);
							local_tile_id = global_tile_id - firstgid;
						}
						else break;
					}
					
					//Assign the proper texture
					if (m_tilesets.find(ts_name) != m_tilesets.end())
						ts_tex = &m_tilesets[ts_name];
					else {
						cerr << "Warning: Tile ID " << global_tile_id << " has no matching tileset! Assigning Default" << endl;
						ts_tex = &m_tilesets["Default"];
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
					const int tiles_per_row = tileset_size.x / TS;
					const int tu = local_tile_id & (tiles_per_row - 1);
					const int tv = local_tile_id / tiles_per_row;

					//Assign vertices for rendering
					if (m_vertices_by_tileset.count(ts_name) == 0) {
						cerr << "Error: m_vertices_by_tileset does not contain " << ts_name << endl;
						continue;
					}
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

		//Resize and populate node grid
		grid.resize(map_size_t.x, vector<Node>(map_size_t.y));
		PopulateNodeGrid();
		
		map_loaded = true;
		return true;
	}

	bool Loaded() const { return map_loaded; }

	sf::Vector2u GetMapSizeTiles() const { return map_size_t; }
	sf::Vector2f GetMapSizePixels() const { return map_size_p; }

	Tile GetTileData(sf::Vector2f tile_pos) { return tile_data[floor(tile_pos.x)][floor(tile_pos.y)]; }


	//Pathfinding
	void PopulateNodeGrid() {
		for (uint row = 0; row < map_size_t.x; ++row) {
			for (uint col = 0; col < map_size_t.y; ++col) {
				/*
				sf::Vector2i pos;
				bool debug;
				bool walkable;
				//TO-DO: flyable, swimmable
				uint cost = 1.f; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
				float g = 0.f (actual cost to get to node), h = 0.f (heuristic cost to node), f = 0.f (g+h, used to select the next node in the path (smaller is better));
				Node* parent = nullptr;
				*/

				bool node_walk = tile_data[row][col].terrain != Terrains::WATER;

				uint node_cost = (tile_data[row][col].terrain == Terrains::ROUGH) + 1;

				grid[row][col] = { sf::Vector2i(row * TS + TS * .5, col * TS + TS * .5), false, node_walk, node_cost };
			}
		}
	}

	float Heuristic(const sf::Vector2i& a, const sf::Vector2i& b) {
		uint dx = abs(a.x - b.x);
		uint dy = abs(a.y - b.y);
		return (dx + dy) + (1.414f - 2) * min(dx, dy);
	}

	struct CompareNodes {
		bool operator()(Node* a, Node* b) {
			return a->f > b->f; // Min-heap, lower f-value has higher priority
		}
	};

	queue<sf::Vector2i> FindPath(const sf::Vector2i& start, const sf::Vector2i& goal, sf::RenderWindow& window) {
		//Create our lists
		priority_queue<Node*, vector<Node*>, CompareNodes> open_list;
		unordered_map<Node*, bool> closed_list;

		//Get the grid coords closest to the start and goal
		sf::Vector2i grid_start = sf::Vector2i(round((start.x - TS * .5) / TS), round((start.y - TS * .5) / TS));
		sf::Vector2i grid_goal = sf::Vector2i(round((goal.x - TS * .5) / TS), round((goal.y - TS * .5) / TS));

		// Initialize start node
		Node* start_node = &grid[grid_start.x][grid_start.y];
		start_node->g = 0;
		start_node->h = Heuristic(start, goal);
		start_node->f = start_node->h;
		open_list.push(start_node);

		Node* goal_node = &grid[grid_goal.x][grid_goal.y];

		// Define possible movements           N        NE       E      SE      S        SW       W        NW
		const sf::Vector2i directions[] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };
		
		while (!open_list.empty()) {
			Node* current = open_list.top();
			open_list.pop();

			//Path found
			if (current->pos == goal_node->pos) {
				vector<sf::Vector2i> path;
				path.push_back(sf::Vector2i(goal));
				//We don't need the start, second, or goal node
				while (current->parent and current->parent->parent) { //Ensures we don't add the start node or the second node, which was itself causing weird behaviors
					current = current->parent; //Putting this at the beginning skips the goal node
					//current->debug = true;
					path.push_back(current->pos);
				}
				
				reverse(path.begin(), path.end());
				queue<sf::Vector2i> path_q;
				for (const auto& p : path)
					path_q.push(p);
				return path_q;
			}

			//Add the current node position to the closed list
			closed_list[current] = true;

			for (const auto& dir : directions) {
				sf::Vector2i neighbor_pos = sf::Vector2i(current->pos.x + dir.x * (int)TS, current->pos.y + dir.y * (int)TS);
				//Out of bounds
				if (neighbor_pos.x <= 0 || neighbor_pos.y <= 0 || neighbor_pos.x >= map_size_p.x || neighbor_pos.y >= map_size_p.y)
					continue;

				Node* neighbor = &grid[neighbor_pos.x / (int)TS][neighbor_pos.y / (int)TS];
				//Unwalkable or already processed
				if (!neighbor->walkable || closed_list[neighbor])
					continue;

				float tentative_g = current->g + neighbor->cost;
				//Better path found
				if (tentative_g < neighbor->g || neighbor->g == 0) {
					neighbor->parent = current;
					neighbor->g = tentative_g;
					neighbor->h = Heuristic(neighbor_pos, goal);
					neighbor->f = neighbor->g + neighbor->h;
					open_list.push(neighbor);
				}
			}
		}

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
			// Check if ts_name exists in m_vertices_by_tileset before accessing it
			if (m_vertices_by_tileset.count(ts_name) > 0)
				target.draw(m_vertices_by_tileset.at(ts_name), states);
		}

		//Draw the node grid
		//NOTE: THIS IS GOING TO RUN LIKE ASS
		/*
		for (uint x = 0; x < grid.size(); ++x) {
			for (uint y = 0; y < grid[x].size(); ++y) {
				if (grid[x][y].walkable) {
					sf::RectangleShape node_box;
					node_box.setSize({ 4, 4 });
					node_box.setFillColor(sf::Color(255, 0, 0, 255));
					node_box.setPosition(sf::Vector2f(grid[x][y].pos.x - 2, grid[x][y].pos.y - 2));

					if (grid[x][y].debug)
						node_box.setFillColor(sf::Color(0, 0, 255, 255));

					target.draw(node_box, states);
				}
			}
		}
		*/
		/* Draw a grid of the tile data (THIS IS ALSO GOING TO RUN LIKE ASS)
		for (uint x = 0; x < tile_data.size(); ++x) {
			for (uint y = 0; y < tile_data[x].size(); ++y) {
				sf::RectangleShape tile_box;
				tile_box.setSize({ 16, 16 });
				tile_box.setFillColor(sf::Color(255, 255, 255, 255*.6)); //White, ~2/3 opacity
				if (tile_data[x][y].terrain == Terrains::WATER)
					tile_box.setFillColor(sf::Color(0, 0, 255, 255 * .7)); //Blue, ~3/4 opacity
				tile_box.setPosition(sf::Vector2f(x*16, y*16));

				target.draw(tile_box, states);
			}
		}
		*/
	}
};