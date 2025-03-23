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
		uint cost = 1.f; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
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
		m_tilesets.emplace("Default", sf::Texture("assets/Sprites/Environments/TileSets/Default.png"));
		if (json_file == "Tutton") {
			m_tilesets.emplace("Grass", sf::Texture("assets/Sprites/Environments/TileSets/Grass.png"));
			m_tilesets.emplace("Stone", sf::Texture("assets/Sprites/Environments/TileSets/Stone.png"));
			m_tilesets.emplace("Water", sf::Texture("assets/Sprites/Environments/TileSets/Water.png"));
			m_tilesets.emplace("Wood", sf::Texture("assets/Sprites/Environments/TileSets/Wood.png"));
		}

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
					else
						ts_tex = &m_tilesets["Default"];
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
		uint x = map_size_t.x - 1;
		uint y = map_size_t.y - 1;
		grid.resize(x, vector<Node>(y));
		for (uint row = 1; row < x; ++row) {
			for (uint col = 1; col < y; ++col) {
				/*
				sf::Vector2i pos;
				bool walkable;
				//TO-DO: flyable, swimmable
				uint cost = 1.f; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
				float g = 0.f (actual cost to get to node), h = 0.f (heuristic cost to node), f = 0.f (g+h, used to select the next node in the path (smaller is better)); //For A* calculations
				Node* parent = nullptr;
				*/

				bool node_walk = tile_data[row - 1][col - 1].terrain != Terrains::WATER or
								tile_data[row + 1][col - 1].terrain != Terrains::WATER or
								tile_data[row - 1][col + 1].terrain != Terrains::WATER or
								tile_data[row + 1][col + 1].terrain != Terrains::WATER;
				uint num_rough = 0;
				if (tile_data[row - 1][col - 1].terrain == Terrains::ROUGH) ++num_rough;
				if (tile_data[row + 1][col - 1].terrain == Terrains::ROUGH) ++num_rough;
				if (tile_data[row - 1][col + 1].terrain == Terrains::ROUGH) ++num_rough;
				if (tile_data[row + 1][col + 1].terrain == Terrains::ROUGH) ++num_rough;
				uint node_cost = (num_rough > 2)+1;
				uint r = row - 1; uint c = col - 1;
				grid[r][c] = { sf::Vector2i(r*16, c*16), false, node_walk, node_cost};
					
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
	float Heuristic(const sf::Vector2i& a, const sf::Vector2i& b) {
		uint dx = abs(a.x - b.x);
		uint dy = abs(a.y - b.y);
		return (dx + dy) + (1.414f - 2) * min(dx, dy);
	}

	queue<sf::Vector2i> FindPath(const sf::Vector2i& start, const sf::Vector2i& goal, sf::RenderWindow& window) {
		//auto cmp = [](const Node* a, const Node* b) { return a->f > b->f; };
		//queue<Node*, vector<Node*>> open_list;
		queue<Node*> open_list;
		unordered_map<Node*, bool> closed_list;

		// Initialize start node
		Node* start_node = &grid[start.x][start.y];
		start_node->g = 0;
		start_node->h = Heuristic(start, goal);
		start_node->f = start_node->h;
		open_list.push(start_node);


		cout << "Player pos: " << sf::Vector2f(start.x * TS, start.y * TS) << endl;
		cout << "Start node pos: " << start_node->pos << endl;

		cout << "Goal pos: " << sf::Vector2f(goal.x * TS, goal.y * TS) << endl;
		Node* goal_node = &grid[goal.x][goal.y];
		cout << "Goal node pos: " << goal_node->pos << endl;


		// Define possible movements           N        NE       E      SE      S        SW       W        NW
		const sf::Vector2i directions[] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };
		
		uint nodes_runthrough = 0;

		while (!open_list.empty()) {
			Node* current = open_list.front();
			open_list.pop();

			nodes_runthrough++;
			/*if (nodes_runthrough % 1000 == 0) {
				cout << "Current node pos: " << current->pos << endl;
				cout << "Current node #: " << nodes_runthrough << endl;
				
				cout << endl;
			}*/
			//Path found
			if (current->pos == goal) {

				cout << "TileMap Path found" << endl;

				vector<sf::Vector2i> path;
				while (current) {
					path.push_back(current->pos);
					current = current->parent;
				}
				reverse(path.begin(), path.end());
				queue<sf::Vector2i> path_q;
				for (const auto& p : path)
					path_q.push(p);
				return path_q;
			}

			//Add the current node position to the closed list
			closed_list[current] = true;
			current->debug = true;


			for (const auto& dir : directions) {
				sf::Vector2i neighbor_pos = current->pos + dir*(int)TS;
				//Out of bounds
				if (neighbor_pos.x < 0 || neighbor_pos.y < 0 || neighbor_pos.x >= map_size_p.x || neighbor_pos.y >= map_size_p.y)
					continue;
				sf::Vector2i n_grid_pos = { neighbor_pos.x / (int)TS, neighbor_pos.y / (int)TS };
				//cout << "Neighbor grid pos: " << n_grid_pos << endl;
				Node* neighbor = &grid[neighbor_pos.x/(int)TS][neighbor_pos.y/(int)TS];
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
		cout << "Tilemap.h draw L 354" << endl;
		//Iterate over each tileset and draw only the vertices using that texture
		for (const auto& [ts_name, ts_tex] : m_tilesets) {
			cout << "Inside for loop" << endl;
			states.texture = &ts_tex;

			//Find which vertices belong to this tileset
			target.draw(m_vertices_by_tileset.at(ts_name), states);
		}
		cout << "Tilemap.h draw L362" << endl;

		//Draw the nodes for testing reasons
		//NOTE: THIS IS GOING TO RUN LIKE ASS
		/*
		for (uint x = 0; x < grid.size(); ++x) {
			for (uint y = 0; y < grid[x].size(); ++y) {
				if (grid[x][y].walkable) {
					sf::RectangleShape node_box;
					node_box.setSize({ 4, 4 });
					node_box.setFillColor(sf::Color(255, 0, 0, 255));
					node_box.setPosition(sf::Vector2f(grid[x][y].pos.x - 2, grid[x][y].pos.y - 2));

					target.draw(node_box, states);
				}
			}
		}
		*/
	}
};