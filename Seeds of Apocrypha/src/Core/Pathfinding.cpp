#include "Pathfinding.h"
#include "Collision.h"
#include "../Graphics/TileMap.h"
#include "../Entities/UI/UI.h" //Entity.h

void Pathfinding::Init(TileMap* t) {
	tmp = t;
	grid.resize(tmp->GetMapSizeTiles().x);
	for (auto& row : grid) row.resize(tmp->GetMapSizeTiles().y);
}

void Pathfinding::PopulateNodeGrid(vector<s_ptr<Entity>>* ents) {
	bool node_walk = false;
	uint node_cost = 0;

	for (int col = 0; col < tmp->GetMapSizeTiles().x; ++col) {
		for (int row = 0; row < tmp->GetMapSizeTiles().y; ++row) {
			node_walk = tmp->GetTileData({ col, row }).terrain != Terrain::Water;
			//For now, we're going to say a node is not walkable if an entity is standing on that tile
			// In the future, this should be *way* more nuanced (i.e. creature vs object, friend or foe, size of entity, etc)
			
			if (node_walk) {
				for (const auto& e : *ents) {
					auto ui = dynamic_cast<UI*>(e.get());
					if (!ui)
						node_walk = !(Collision::Point(e->GetPos(), Rect({ col * TS, row * TS }, TS)));
					
					if (!node_walk) break;
				}
			}
			

			node_cost = (tmp->GetTileData({col, row}).terrain == Terrain::Rough) + 1;

			grid[col][row] = { Round(col * TS + TS * .5f, row * TS + TS * .5f), false, node_walk, node_cost };
		}
	}
}

queue<Vector2i> Pathfinding::FindPath(const Vector2i& start, const Vector2i& goal) {
	//Create our lists
	std::priority_queue<Node*, vector<Node*>, CompareNodes> open_list;
	unordered_map<Node*, bool> closed_list;

	//Get the grid coords closest to the start and goal
	Vector2i grid_start = Round((start.x - TS * .5f) / TS, (start.y - TS * .5f) / TS);
	Vector2i grid_goal = Round((goal.x - TS * .5f) / TS, (goal.y - TS * .5f) / TS);

	// Initialize start node
	Node* start_node = &grid[grid_start.x][grid_start.y];
	start_node->g = 0;
	start_node->h = Heuristic(start, goal);
	start_node->f = start_node->h;
	open_list.push(start_node);

	Node* goal_node = &grid[grid_goal.x][grid_goal.y];

	// Define possible movements      N        NE       E       SE      S        SW       W        NW
	const Vector2i directions[] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };

	while (!open_list.empty()) {
		Node* current = open_list.top();
		open_list.pop();

		//Path found
		if (current->pos == goal_node->pos) {
			vector<Vector2i> path;
			//path.push_back(goal); //Decided to remove the goal node (where the mouse clicked) for more grid-based movement
			//We don't need the start or second
			while (current->parent and current->parent->parent) { //Ensures we don't add the start node or the second node, which was itself causing weird behaviors
				//current->debug = true;
				path.push_back(current->pos);
				current = current->parent; //Putting this at the beginning will skip the goal node
			}

			reverse(path.begin(), path.end());
			queue<Vector2i> path_q;
			for (const auto& p : path)
				path_q.push(p);
			return path_q;
		}

		//Add the current node position to the closed list
		closed_list[current] = true;

		for (const auto& dir : directions) {
			Vector2i neighbor_pos = current->pos + dir * (int)TS;
			//Out of bounds
			if (neighbor_pos.x <= 0 || neighbor_pos.y <= 0 || neighbor_pos.x >= tmp->GetMapSizePixels().x || neighbor_pos.y >= tmp->GetMapSizePixels().y)
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

float Pathfinding::Heuristic(const Vector2i& a, const Vector2i& b) {
	uint dx = abs(a.x - b.x);
	uint dy = abs(a.y - b.y);
	return (dx + dy) + (sqrt2 - 2) * std::min(dx, dy);
}
