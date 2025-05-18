#include "Pathfinding.h"

void Pathfinding::PopulateNodeGrid(vector<s_ptr<Entity>>* ents) {
	for (int col = 0; col < tmp->GetMapSizeTiles().x; ++col) {
		for (int row = 0; row < tmp->GetMapSizeTiles().y; ++row) {
			bool node_walk = tmp->GetTileData({ col, row }).terrain != Terrains::WATER;

			uint node_cost = (tmp->GetTileData({col, row}).terrain == Terrains::ROUGH) + 1;

			grid[col][row] = { {row * TS + TS * .5, col * TS + TS * .5}, false, node_walk, node_cost };
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
			path.push_back(Vector2i(goal));
			//We don't need the start, second, or goal node
			while (current->parent and current->parent->parent) { //Ensures we don't add the start node or the second node, which was itself causing weird behaviors
				current = current->parent; //Putting this at the beginning skips the goal node
				//current->debug = true;
				path.push_back(current->pos);
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
	return (dx + dy) + (1.414f - 2) * std::min(dx, dy);
}
