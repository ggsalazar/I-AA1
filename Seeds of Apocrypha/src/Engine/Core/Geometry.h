#pragma once
#include "Vector2.h"

namespace Engine {

struct Color {
	//Values between 0 and 1
	float r{}, g{}, b{}, a{};

	Color(float rgb) : r(rgb), g(rgb), b(rgb), a(1) {}
	Color(float r, float g, float b) : r(r), g(g), b(b), a(1) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Color& c) { return os << "r: " << c.r << " g: " << c.g << " b: " << c.b << " a: " << c.a; }

struct Line {
	Vector2u pos1, pos2;
	uint w{};

	Line(Vector2u p1, Vector2u p2) : pos1(p1), pos2(p2), w(1) {}
	Line(Vector2u p1, Vector2u p2, uint w) : pos1(p1), pos2(p2), w(w) {}
	inline float Length() const { return sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y)); }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Line& l) { return os << "p1: " << l.pos1 << " p2: " << l.pos2; }

struct Circle {
	Vector2u pos;
	float r;

	Circle() : pos({ 0, 0 }), r(0) {}
	Circle(Vector2u p, float r) : pos(p), r(r) {}

	inline float Area() const { return r * r * 3.14159; }
	inline float Circ() const { return 6.28318 * r; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Circle& c) { return os << "Radius: " << c.r; }

struct Tri {
	Vector2u pos1, pos2, pos3;

	Tri(Vector2u p1, Vector2u p2, Vector2u p3) : pos1(p1), pos2(p2), pos3(p3) {}

	inline float Area() const { return (pos1.x * (pos2.y - pos3.y) + pos2.x * (pos3.y - pos1.y) + pos3.x * (pos1.y - pos2.y)) * .5; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Tri& t) { return os << "p1: " << t.pos1 << " p2: " << t.pos2 << " p3: " << t.pos3; }

struct Rect {
	Vector2u pos, size;

	Rect(Vector2u p, uint s) : pos(p), size({s,s}) {}
	Rect(Vector2u p, Vector2u s) : pos(p), size(s) {}

	inline float Area() const { return size.x * size.y; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect& r) { return os << "pos: " << r.pos << " size: " << r.size; }

}