#pragma once
#include "Geometry.h"

class Camera {
public:
	//Viewport x/y is camera pos, pos+size is current view area
	Rect viewport;

	Camera() = default;
	Camera(Vector2i pos) { viewport.x = pos.x; viewport.y = pos.y; }
	Camera(Vector2i pos, Vector2i size) { viewport.x = pos.x; viewport.y = pos.y; viewport.w = size.x; viewport.h = size.y; }

	void MoveTo(Vector2i new_pos) { viewport.x = new_pos.x; viewport.y = new_pos.y; }
	void MoveBy(Vector2i offset) { viewport.x += offset.x; viewport.y += offset.y; }
};