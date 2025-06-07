#include "Pathfinding.h"
#include "Collision.h"
#include "../Entities/UI/UI.h" //Entity.h

void Pathfinding::Init(TileMap* t) {
	tmp = t;
	grid.resize(tmp->GetMapSizeTiles().x);
	for (auto& row : grid) row.resize(tmp->GetMapSizeTiles().y);
}

void Pathfinding::PopulateNodeGrid(vector<Entity*>* ents) {
	bool node_walk = false;
	uint node_cost = 0;

	for (int col = 0; col < tmp->GetMapSizeTiles().x; ++col) {
		for (int row = 0; row < tmp->GetMapSizeTiles().y; ++row) {
			node_walk = tmp->GetTileData({ col, row }).terrain != Terrain::Water;
			//For now, we're going to say a node is not walkable if an entity is standing on that tile
			// In the future, this should be *way* more nuanced (i.e. creature vs object, friend or foe, size of entity, etc)
			
			if (node_walk) {
				for (const auto& e : *ents) {
					if (!dynamic_cast<UI*>(e))
						node_walk = !(Collision::RectPoint(Rect({ col * TS, row * TS }, TS), e->GetPos()));
					
					if (!node_walk) break;
				}
			}
			

			node_cost = (tmp->GetTileData({col, row}).terrain == Terrain::Rough) + 1;

			grid[col][row] = { Round(col * TS + TS * .5f, row * TS + TS * .5f), false, node_walk, false, node_cost };
		}
	}
}

vector<Vector2i> Pathfinding::FindPath(const Vector2i& start, Vector2i& goal, Entity* target) {
	
	//If the target is sufficiently close to the goal and the goal is not a tile,
	// there is no need to find a path - TO-DO
	if (target and Distance(start, goal) <= 1.5 * METER) return {};
	
	//Create our lists
	std::priority_queue<Node*, vector<Node*>, CompareNodes> open_list;
	unordered_map<Node*, bool> closed_list;

	//Get the grid coords closest to the start and goal
	Vector2i grid_start = Round((start.x - TS * .5f) / TS, (start.y - TS * .5f) / TS);
	//Select the closest unoccupied and unclaimed node to the start node
	Vector2i grid_goal = Round((goal.x - TS * .5f) / TS, (goal.y - TS * .5f) / TS);
	Vector2i closest_node = grid_goal;
	if (target) {
		for (int i = grid_goal.x - 1; i <= grid_goal.x + 1; ++i) {
			for (int j = grid_goal.y - 1; j <= grid_goal.y + 1; ++j) {
				Node* n = &grid[i][j];

				if (n and n->walkable and !n->claimed and Distance(n->pos, grid[grid_start.x][grid_start.y].pos) < Distance(grid[closest_node.x][closest_node.y].pos, grid[grid_start.x][grid_start.y].pos))
					closest_node = { i, j };
			}
		}

		//If the closest node we found was the goal node, that means we didn't find a closer tile to stand on,
		// which means we have no path to the target
		if (closest_node == grid_goal) return {};

		goal = (closest_node + TS * .5f) * TS;
	}

	// Initialize start node
	Node* start_node = &grid[grid_start.x][grid_start.y];
	start_node->g = 0;
	start_node->h = Heuristic(start, goal);
	start_node->f = start_node->h;
	open_list.push(start_node);

	Node* goal_node = &grid[closest_node.x][closest_node.y];

	//Define possible movements       N        NE       E       SE      S        SW       W        NW
	const Vector2i directions[] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };

	//Find the path
	while (!open_list.empty()) {
		Node* current = open_list.top();
		open_list.pop();

		//Path found
		if (current->pos == goal_node->pos) {
			//This node is claimed
			current->claimed = true;

			vector<Vector2i> path;
			//path.push_back(goal); //Decided to remove the goal node (where the mouse clicked) for more grid-based movement
			//We don't need the start or second
			while (current->parent and current->parent->parent) { //Ensures we don't add the start node or the second node, which was causing weird behaviors
				current->debug = true;
				path.push_back(current->pos);
				current = current->parent; //Putting this at the beginning will skip the goal node
			}

			reverse(path.begin(), path.end());
			//vector<Vector2i> path_q;
			//for (const auto& p : path)
				//path_q.push_back(p);
			return path;
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

queue<Vector2i> Pathfinding::SmoothPath(const vector<Vector2i>& raw) {
	queue<Vector2i> smoothed;
	uint i = 0, j;

	while (i < raw.size() - 1) {
		j = raw.size() - 1;
		//Find the farthest node we can reach directly
		while (j > i + 1) {
			if (LineOfSight(raw[i], raw[j]))
				break;
			--j;
		}
		smoothed.push(raw[j]);
		i = j;
	}

	return smoothed;
}

float Pathfinding::Heuristic(const Vector2i& a, const Vector2i& b) {
	uint dx = abs(a.x - b.x);
	uint dy = abs(a.y - b.y);
	return (dx + dy) + (sqrt2 - 2) * std::min(dx, dy);
}

bool Pathfinding::LineOfSight(const Vector2i& from, const Vector2i& to) {
	//Bresenham’s Line Algorithm or DDA
	//Return false if any tile along the line is unwalkable
	//Simplified version for now (assumes TS-based grid)

	Vector2i delta = to - from;
	int steps = std::max(abs(delta.x), abs(delta.y)) / TS;
	Vector2f dir = Vector2f(delta.x, delta.y) / (float)steps; 

	Vector2i point;
	for (int i = 1; i < steps; ++i) {
		point = { (int)(from.x + dir.x * i) / TS, (int)(from.y + dir.y * i) / TS };
		if (!grid[point.x][point.y].walkable)
			return false;
	}


	/*Vector2i pointi;
	Vector2f point;
	for (int i = 1; i < steps; ++i) {
		point = Vector2f(from.x, from.y) + dir * (float)i;
		pointi.x = (int)(point.x / TS);
		pointi.y = (int)(point.y / TS);
		if (!grid[pointi.x][pointi.y].walkable)
			return false;
	}*/

	return true;
}