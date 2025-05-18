#pragma once
#include <vector>
#include <queue>
#include "../Graphics/TileMap.h"
#include "Vector2.h"

using std::queue, std::vector;

class Entity;

class Pathfinding {
public:
	struct Node {
		Vector2i pos = { 0, 0 };
		bool debug = false; //Activate if in the closed list
		bool walkable = false;
		//TO-DO: flyable, swimmable
		uint cost = 1; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
		float g = 0.f, h = 0.f, f = 0.f; //For A* calculations
		Node* parent = nullptr;
	};

	Pathfinding() = default;
	void Init(TileMap* t) { tmp = t; grid.resize(tmp->GetMapSizeTiles().x); for (auto& row : grid) row.resize(tmp->GetMapSizeTiles().y); }
	void PopulateNodeGrid(vector<s_ptr<Entity>>* ents);

	queue<Vector2i> FindPath(const Vector2i& start, const Vector2i& goal);

private:
	vector<vector<Node>> grid;

	TileMap* tmp = nullptr;

	struct CompareNodes {
		bool operator()(Node* a, Node* b) {
			return a->f > b->f; // Min-heap, lower f-value has higher priority
		}
	};

	float Heuristic(const Vector2i& a, const Vector2i& b);
};