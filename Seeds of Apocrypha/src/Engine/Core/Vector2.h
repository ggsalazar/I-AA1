#pragma once
#include <iostream>
#include <cmath>

using uint = unsigned int;

template <typename T = float>
struct Vector2 {
	T x, y;

	//Constructors
	constexpr Vector2() : x(0), y(0) {}
    constexpr Vector2(T x) : x(x), y(x) {}
	constexpr Vector2(T x, T y) : x(x), y(y) {}
    constexpr Vector2(Vector2<T> v) : x(v.x), y(v.y) {}

    //Addition
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2& operator+=(const Vector2& other) {
        x += other.x; y += other.y;
        return *this;
    }

    //Subtraction
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2& operator-=(const Vector2& other) {
        x -= other.x; y -= other.y;
        return *this;
    }

    //Multiplication
    Vector2 operator*(T scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    Vector2 operator*(Vector2<T> other) const {
        return Vector2(x * other.x, y * other.y);
    }
    Vector2& operator*=(T scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }
    template<typename U>
    Vector2<T> operator*(Vector2<U> other) const {
        return Vector2<T>(static_cast<T>(x) * static_cast<T>(other.x),
                          static_cast<T>(y) * static_cast<T>(other.y));
    }


    //Division
    Vector2 operator/(T scalar) const {
        return Vector2(x / scalar, y / scalar);
    }
    Vector2 operator/(Vector2<T> other) const {
        return Vector2(x / other.x, y / other.y);
    }
    Vector2& operator/=(T scalar) {
        x /= scalar; y /= scalar;
        return *this;
    }
    template<typename U>
        Vector2<T> operator/(Vector2<U> other) const {
        return Vector2<T>(static_cast<T>(x) / static_cast<T>(other.x),
                          static_cast<T>(y) / static_cast<T>(other.y));
    }


    //(In)Equality
    bool operator==(const Vector2& other) const {
        return this->x == other.x and this->y == other.y;
    }
    bool operator!=(const Vector2& other) const {
        return this->x != other.x or this->y != other.y;
    }

    //Magnitude (Length)
    T Length() const { return std::sqrt(x * x + y * y); }

    //Normalize
    Vector2 Normalized() const {
        T len = Length();
        return (len != 0) ? *this / len : *this;
    }

    //Dot product
    T Dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }



};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) { return os << vec.x << ", " << vec.y; }

using Vector2u = Vector2<unsigned int>;
using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;