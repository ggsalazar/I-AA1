#pragma once
#include <vector>
#include <queue>
#include "Aliases.h"
#include "Vector2.h"
#include "../Graphics/TileMap.h"

using std::queue, std::vector;

class Entity;
class TileMap;

class Pathfinding {
	friend class Renderer;
public:
	struct Node {
		Vector2i pos = { 0, 0 };
		bool debug = false; //Activate if in the closed list
		bool walkable = false;
		//TO-DO: flyable, swimmable
		bool claimed = false; //True when a creature has this node as a goal; doesn't affect pathfinding, just goal setting
		uint cost = 1; //1 for normal ground, 2 for rough terrain, 3 for slightly dangerous, 4 for moderately dangerous, 5 for highly dangerous
		float g = 0.f, h = 0.f, f = 0.f; //For A* calculations
		Node* parent = nullptr;
	};

	Pathfinding() = default;
	void Init(TileMap* t);
	void PopulateNodeGrid(vector<Entity*>* ents);

	queue<Vector2i> FindPath(const Vector2i& start, Vector2i& goal, MouseTarget target);

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