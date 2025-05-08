#pragma once
#include <iostream>
#include <cmath>

#define VEC2_INLINE [[nodiscard]] constexpr inline

using uint = unsigned int;

template <typename T = float>
struct Vector2 {
	T x, y;

	//Constructors
	constexpr Vector2() : x(0), y(0) {}
    constexpr Vector2(T x) : x(x), y(x) {}
	constexpr Vector2(T x, T y) : x(x), y(y) {}

    //Addition
    VEC2_INLINE Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    VEC2_INLINE Vector2& operator+=(const Vector2& other) {
        x += other.x; y += other.y;
        return *this;
    }

    //Subtraction
    VEC2_INLINE Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    VEC2_INLINE Vector2& operator-=(const Vector2& other) {
        x -= other.x; y -= other.y;
        return *this;
    }

    //Multiplication
    VEC2_INLINE Vector2 operator*(T scalar) const { return Vector2(x * scalar, y * scalar); }
    VEC2_INLINE Vector2& operator*=(T scalar) {
        x *= scalar; y *= scalar;
        return *this;
    }
    template<typename U>
    VEC2_INLINE Vector2 operator*(Vector2<U> other) const {
        return Vector2<T>(x * static_cast<T>(other.x), y * static_cast<T>(other.y));
    }

    //Division
    VEC2_INLINE Vector2 operator/(T scalar) const { return Vector2(x / scalar, y / scalar); }
    VEC2_INLINE Vector2& operator/=(T scalar) {
        x /= scalar; y /= scalar;
        return *this;
    }
    template<typename U>
    VEC2_INLINE Vector2<T> operator/(Vector2<U> other) const {
        return Vector2<T>(x / static_cast<T>(other.x), y / static_cast<T>(other.y));
    }


    //(In)Equality
    VEC2_INLINE bool operator==(const Vector2& other) const { return x == other.x and y == other.y; }
    VEC2_INLINE bool operator!=(const Vector2& other) const { return x != other.x or y != other.y; }

    //Magnitude (Length)
    [[nodiscard]] constexpr T Length() const { return std::sqrt(x * x + y * y); }
    //Normalize
    [[nodiscard]] constexpr Vector2 Normalized() const {
        T len = Length();
        return (len != 0) ? *this / len : *this;
    }
    //Dot product
    [[nodiscard]] constexpr T Dot(const Vector2& other) const { return x * other.x + y * other.y; }
};
//ostream operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec) { return os << vec.x << ", " << vec.y; }

//Universal scalar multiplication & division
template<typename T>
Vector2<T> operator*(T scalar, const Vector2<T>& vec) { return vec * scalar; }
template<typename T>
Vector2<T> operator/(T scalar, const Vector2<T>& vec) { return vec / scalar; }

using Vector2u = Vector2<unsigned int>;
using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;