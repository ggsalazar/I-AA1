#pragma once
#include <iostream>
#include <cmath>

template <typename T = float>
class Vector3 {
public:
	T x, y, z;

	//Constructors
	constexpr Vector3() : x(0), y(0), z(0) {}
	constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
};

//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) { return os << v.x << ", " << v.y << ", " << v.z; }

using Vector3u = Vector3<unsigned int>;
using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;