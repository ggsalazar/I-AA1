#pragma once
#include "Vector2.h" //iostream

struct Color {
	//Values between 0 and 1
	float r = 0.f, g = 0.f, b = 0.f, a = 0.f;

	Color() = default;
	Color(float rgb) : r(rgb), g(rgb), b(rgb), a(1) {}
	Color(float r, float g, float b) : r(r), g(g), b(b), a(1) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Color& c) { return os << "r: " << c.r << " g: " << c.g << " b: " << c.b << " a: " << c.a; }

struct Line {
	Vector2i pos1, pos2;

	Line() = default;
	Line(Vector2i p1, Vector2i p2) : pos1(p1), pos2(p2) {}
	inline float Length() const { return sqrt((pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y)); }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Line& l) { return os << "p1: " << l.pos1 << " p2: " << l.pos2; }

struct Circle {
	int x=0, y=0;
	float r = 0;

	Circle(Vector2i p, float r) : x(p.x), y(p.y), r(r) {}

	inline float Area() const { return r * r * 3.14159; }
	inline float Circ() const { return 6.28318 * r; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Circle& c) { return os << "Circle pos: " << Vector2i{c.x, c.y} << "; Radius: " << c.r; }

struct Tri {
	Vector2i pos1, pos2, pos3;

	Tri(Vector2i p1, Vector2i p2, Vector2i p3) : pos1(p1), pos2(p2), pos3(p3) {}

	inline float Area() const { return (pos1.x * (pos2.y - pos3.y) + pos2.x * (pos3.y - pos1.y) + pos3.x * (pos1.y - pos2.y)) * .5; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Tri& t) { return os << "p1: " << t.pos1 << " p2: " << t.pos2 << " p3: " << t.pos3; }

struct Rect {
	int x=0, y=0, w=0, h=0;

	Rect() = default;
	Rect(Vector2i p, int s) : x(p.x), y(p.y), w(s), h(s) {}
	Rect(Vector2i p, Vector2i s) : x(p.x), y(p.y), w(s.x), h(s.y) {}

	inline float Area() const { return w * h; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Rect& r) { return os << "pos: " << Vector2i{r.x, r.y} << " size: " << Vector2i{r.w, r.h}; }