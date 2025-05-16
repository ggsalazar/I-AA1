#pragma once
#include <vector>
#include <queue>
#include "../Graphics/TileMap.h"
#include "Vector2.h"

class Entity;

class Pathfinding {
public:
	struct Node {
		Vector2u pos = { 0, 0 };
		bool debug = false; //Activate if in the closed list
		bool walkable = false;
		//TO-DO: flyable, swimmable
		uint cost = 1; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
		float g = 0.f, h = 0.f, f = 0.f; //For A* calculations
		Node* parent = nullptr;
	};

	void PopulateNodeGrid(TileMap* tmp, vector<s_ptr<Entity>>* ents) {
		for (uint row = 0; row < map_size_t.x; ++row) {
			for (uint col = 0; col < map_size_t.y; ++col) {
				bool node_walk = tile_data[row][col].terrain != Terrains::WATER;

				uint node_cost = (tile_data[row][col].terrain == Terrains::ROUGH) + 1;

				grid[row][col] = { Vector2i(row * TS + TS * .5, col * TS + TS * .5), false, node_walk, node_cost };
			}
		}
	}

	queue<Vector2u> FindPath(const Vector2u& start, const Vector2u& goal, sf::RenderWindow& window) {
		//Create our lists
		priority_queue<Node*, vector<Node*>, CompareNodes> open_list;
		unordered_map<Node*, bool> closed_list;

		//Get the grid coords closest to the start and goal
		Vector2u grid_start = { round((start.x - TS * .5) / TS), round((start.y - TS * .5) / TS) };
		Vector2u grid_goal = { round((goal.x - TS * .5) / TS), round((goal.y - TS * .5) / TS) };

		// Initialize start node
		Node* start_node = &grid[grid_start.x][grid_start.y];
		start_node->g = 0;
		start_node->h = Heuristic(start, goal);
		start_node->f = start_node->h;
		open_list.push(start_node);

		Node* goal_node = &grid[grid_goal.x][grid_goal.y];

		// Define possible movements           N        NE       E      SE      S        SW       W        NW
		const Vector2i directions[] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };

		while (!open_list.empty()) {
			Node* current = open_list.top();
			open_list.pop();

			//Path found
			if (current->pos == goal_node->pos) {
				vector<Vector2u> path;
				path.push_back(Vector2u(goal));
				//We don't need the start, second, or goal node
				while (current->parent and current->parent->parent) { //Ensures we don't add the start node or the second node, which was itself causing weird behaviors
					current = current->parent; //Putting this at the beginning skips the goal node
					//current->debug = true;
					path.push_back(current->pos);
				}

				reverse(path.begin(), path.end());
				queue<Vector2u> path_q;
				for (const auto& p : path)
					path_q.push(p);
				return path_q;
			}

			//Add the current node position to the closed list
			closed_list[current] = true;

			for (const auto& dir : directions) {
				Vector2i neighbor_pos = current->pos + dir * TS;
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
	vector<vector<Node>> grid;

	struct CompareNodes {
		bool operator()(Node* a, Node* b) {
			return a->f > b->f; // Min-heap, lower f-value has higher priority
		}
	};

	float Heuristic(const Vector2u& a, const Vector2u& b) {
		uint dx = abs(a.x - b.x);
		uint dy = abs(a.y - b.y);
		return (dx + dy) + (1.414f - 2) * min(dx, dy);
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
					node_box.setPosition(Vector2f(grid[x][y].pos.x - 2, grid[x][y].pos.y - 2));

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
				tile_box.setPosition(Vector2f(x*16, y*16));

				target.draw(tile_box, states);
			}
		}
		*/
}
