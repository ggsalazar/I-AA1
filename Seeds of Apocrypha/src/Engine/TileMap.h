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
	Terrains terrain;
	float elevation;
};


class TileMap : public sf::Drawable, public sf::Transformable {
public:

	struct Node {
		sf::Vector2u pos;
		bool walkable;
		//TO-DO: flyable, swimmable
		uint cost = 1.f; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
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


		//Load all of our textures only once
		sf::Texture grass_tex("assets/Sprites/Environments/TileSets/Grass.png");
		sf::Texture stone_tex("assets/Sprites/Environments/TileSets/Stone.png");
		sf::Texture water_tex("assets/Sprites/Environments/TileSets/Water.png");
		sf::Texture wood_tex("assets/Sprites/Environments/TileSets/Wood.png");

		//Populate vertex array
		for (const auto& layer : tilemap_data["layers"]) {
			for (unsigned int row = 0; row < map_size_t.x; ++row) {
				for (unsigned int col = 0; col < map_size_t.y; ++col) {
					//Get current tile ID number
					const unsigned int global_tile_id = layer["data"][row + col * map_size_t.x];

					//Skip empty tiles
					if (!global_tile_id) continue;

					//Identify the correct tileset for this tile
					sf::Texture* ts_tex = nullptr;
					unsigned int local_tile_id = global_tile_id;

					for (const auto& tileset : tilemap_data["tilesets"]) {
						const unsigned int firstgid = tileset["firstgid"];
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

					//Emplace our textures
					if (m_tilesets.find(ts_name) == m_tilesets.end()) {
						if (ts_name == "Grass")
							m_tilesets.emplace(ts_name, grass_tex);
						else if (ts_name == "Stone")
							m_tilesets.emplace(ts_name, stone_tex);
						else if (ts_name == "Water")
							m_tilesets.emplace(ts_name, water_tex);
						else if (ts_name == "Wood")
							m_tilesets.emplace(ts_name, wood_tex);
					}
					ts_tex = &m_tilesets[ts_name];
					ts_tex->setSmooth(false);

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
					const int tiles_per_row = tileset_size.x / TS; //Equivalent to tileset_size.x / 16
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
		unsigned int x = map_size_t.x - 1;
		unsigned int y = map_size_t.y - 1;
		grid.resize(x, vector<Node>(y));
		for (unsigned int row = 1; row < x-1; ++row) {
			for (unsigned int col = 1; col < y-1; ++col) {
				/*
				sf::Vector2u pos;
				bool walkable;
				//TO-DO: flyable, swimmable
				uint cost = 1.f; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
				float g = 0.f, h = 0.f, f = 0.f; //For A* calculations
				Node* parent = nullptr;
				*/

				bool node_walk = tile_data[row - 1][col - 1].terrain != Terrains::WATER or
								tile_data[row + 1][col - 1].terrain != Terrains::WATER or
								tile_data[row - 1][col + 1].terrain != Terrains::WATER or
								tile_data[row + 1][col + 1].terrain != Terrains::WATER;
				uint node_cost = 1; //NEED TO CALCULATE!

				grid[row - 1][col - 1] = { {row*16, col*16} , node_walk, node_cost};
					
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
		return (dx + dy) + (1.414f - 2) * min(dx, dy);
	}

	queue<sf::Vector2i> FindPath(const sf::Vector2i& start, const sf::Vector2i& goal) {
		auto cmp = [](const Node* a, const Node* b) { return a->f > b->f; }; //I DON'T THINK THIS FUNCTION IS NECESSARY
		//WARNING: CGPT CODE BELOW
		/*
		* 5. Implement A Algorithm*

			Create open and closed lists.
			Start by adding the starting node to the open list.
			While the open list isn’t empty:
				Find node with lowest f in open list.
				Move it to the closed list.
				For each neighbor:
					Skip if in closed list or not walkable.
					Calculate g, h, and f costs.
					If not in open list or a cheaper path is found, update costs and parent pointer.
			Stop if you reach the target node.

		6. Reconstruct Path

			From the target node, backtrack using parent pointers to build the path.

		7. Smooth the Path (Optional)

			Check if segments of the path can be replaced with straight lines avoiding obstacles.

		8. Code Snippet for A (Simplified)*

		Here's a quick sketch of the main loop for A*:

		while (!openList.empty()) {
			Node* current = GetLowestFNode(openList);
			if (current == target) break;

			openList.erase(current);
			closedList.insert(current);

			for (Node* neighbor : GetNeighbors(current)) {
				if (closedList.count(neighbor) || !neighbor->walkable) continue;

				int newG = current->g + GetMovementCost(current, neighbor);
				if (newG < neighbor->g || !InOpenList(neighbor, openList)) {
					neighbor->g = newG;
					neighbor->h = DiagonalHeuristic(neighbor->pos, target->pos);
					neighbor->f = neighbor->g + neighbor->h;
					neighbor->parent = current;

					if (!InOpenList(neighbor, openList))
						openList.insert(neighbor);
				}
			}
		}

		//ORTHO CODE
		std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> open_list(cmp);
		std::unordered_map<sf::Vector2i, bool, Vector2Hash> closed_list;  // Use a hash function for sf::Vector2i keys

		// Initialize start node
		Node* start_node = &grid[start.x][start.y];
		start_node->g = 0;
		start_node->h = Heuristic(start, goal);
		start_node->f = start_node->h;
		open_list.push(start_node);

		// Define possible movements
		const sf::Vector2i directions[] = { {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1} }; 

		while (!open_list.empty()) {
			Node* current = open_list.top();
			open_list.pop();

			if (current->position == goal) {  // Path found
				std::vector<sf::Vector2i> path;
				while (current) {
					path.push_back(current->position);
					current = current->parent;
				}
				std::reverse(path.begin(), path.end());
				return path;
			}

			closed_list[current->position] = true;

			for (const auto& dir : directions) {
				sf::Vector2i neighbor_pos = current->position + dir;
				if (neighbor_pos.x < 0 || neighbor_pos.y < 0 || neighbor_pos.x >= grid.size() || neighbor_pos.y >= grid[0].size())
					continue;  // Out of bounds

				Node* neighbor = &grid[neighbor_pos.x][neighbor_pos.y];
				if (!neighbor->walkable || closed_list[neighbor_pos])
					continue;  // Unwalkable or already processed

				float tentative_g = current->g + neighbor->cost;
				if (tentative_g < neighbor->g || neighbor->g == 0) {  // Better path found
					neighbor->parent = current;
					neighbor->g = tentative_g;
					neighbor->h = Heuristic(neighbor_pos, goal);
					neighbor->f = neighbor->g + neighbor->h;
					open_list.push(neighbor);
				}
			}
		}
		*/


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


		//Draw the nodes for testing reasons
		//NOTE: THIS IS GOING TO RUN LIKE ASS
		/*
		for (unsigned int x = 0; x < grid.size(); ++x) {
			for (unsigned int y = 0; y < grid[x].size(); ++y) {
				sf::RectangleShape node_box;
				node_box.setSize({ 4, 4 });
				node_box.setFillColor(sf::Color(255, 0, 0, 255));
				node_box.setPosition(sf::Vector2f(grid[x][y].pos.x - 2, grid[x][y].pos.y - 2 ));

				target.draw(node_box, states);
			}
		}
		*/
	}
};