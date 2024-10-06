#pragma once
#include <Windows.h>
#include <math.h>
#include <chrono>
#include <cmath>
#include <algorithm>

inline constexpr float radToDegree{ 57.29577951308232087679815 };

class Vec4 {
public:
	float x, y, z, w;
public:
	Vec4() :x(0.f), y(0.f), z(0.f), w(0.f) {}
	Vec4(float x_, float y_, float z_, float w_) :x(x_), y(y_), z(z_), w(w_) {}
};

struct Vec3 {
    float x, y, z;

    // Constructors
    constexpr Vec3(float x = 0.f, float y = 0.f, float z = 0.f) :
        x(x), y(y), z(z) { }

    constexpr Vec3(const float* arrAngles) :
        x(arrAngles[0]), y(arrAngles[1]), z(arrAngles[2]) { }

    // Array index operators
    [[nodiscard]] constexpr float& operator[](int nIndex) {
        return *(&x + nIndex);
    }

    [[nodiscard]] constexpr const float& operator[](int nIndex) const {
        return *(&x + nIndex);
    }

    // Comparison operators
    constexpr bool operator==(const Vec3& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    constexpr bool operator!=(const Vec3& other) const {
        return !(*this == other);
    }

    // Arithmetic assignment operators
    constexpr Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr Vec3& operator*=(const Vec3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    constexpr Vec3& operator/=(const Vec3& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    constexpr Vec3& operator+=(float value) {
        x += value;
        y += value;
        z += value;
        return *this;
    }

    constexpr Vec3& operator-=(float value) {
        x -= value;
        y -= value;
        z -= value;
        return *this;
    }

    constexpr Vec3& operator*=(float value) {
        x *= value;
        y *= value;
        z *= value;
        return *this;
    }

    constexpr Vec3& operator/=(float value) {
        x /= value;
        y /= value;
        z /= value;
        return *this;
    }

    // Unary negation operator
    constexpr Vec3 operator-() const {
        return { -x, -y, -z };
    }

    // Arithmetic operators
    constexpr Vec3 operator+(const Vec3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    constexpr Vec3 operator-(const Vec3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    constexpr Vec3 operator*(const Vec3& other) const {
        return { x * other.x, y * other.y, z * other.z };
    }

    constexpr Vec3 operator/(const Vec3& other) const {
        return { x / other.x, y / other.y, z / other.z };
    }

    constexpr Vec3 operator+(float value) const {
        return { x + value, y + value, z + value };
    }

    constexpr Vec3 operator-(float value) const {
        return { x - value, y - value, z - value };
    }

    constexpr Vec3 operator*(float value) const {
        return { x * value, y * value, z * value };
    }

    constexpr Vec3 operator/(float value) const {
        return { x / value, y / value, z / value };
    }

    // Utility functions
    [[nodiscard]] bool IsValid() const {
        return (std::isfinite(x) && std::isfinite(y) && std::isfinite(z));
    }

    [[nodiscard]] constexpr bool IsZero() const {
        return (x == 0.f && y == 0.f && z == 0.f);
    }

    [[nodiscard]] float Distance() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] float DistanceTo(const Vec3& vec) const {
        return (*this - vec).Distance();
    }

    [[nodiscard]] constexpr Vec3 Normalize() const {
        return *this / Distance();
    }

    [[nodiscard]] constexpr Vec3 ToRad() const {
        return { x / radToDegree, y / radToDegree, z / radToDegree };
    }

    [[nodiscard]] constexpr Vec3 ToDeg() const {
        return { x * radToDegree, y * radToDegree, z * radToDegree };
    }
};

struct Vec2
{
	constexpr Vec2(float x = 0.f, float y = 0.f) :
		x(x), y(y) { }

	constexpr Vec2(const float* arrAngles) :
		x(arrAngles[0]), y(arrAngles[1]) { }

	constexpr Vec2 operator+(const Vec2& vec) const {
		return Vec2(x + vec.x, y + vec.y);
	}

	constexpr Vec2 operator-(const Vec2& vec) const {
		return Vec2(x - vec.x, y - vec.y);
	}

	constexpr Vec2 operator+=(const Vec2& vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	constexpr Vec2 operator-=(const Vec2& vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	constexpr Vec2 operator*(const Vec2& vec) const {
		return Vec2(this->x * vec.x, this->y * vec.y);
	}

	constexpr Vec2 operator*(const float s) const {
		return Vec2(this->x * s, this->y * s);
	}

	constexpr Vec2 operator/(const Vec2& vec) const {
		return Vec2(this->x / vec.x, this->y / vec.y);
	}

	constexpr Vec2 operator/(const float s) const {
		return Vec2(this->x / s, this->y / s);
	}

	constexpr Vec2 operator*=(const Vec2& vec) {
		x *= vec.x;
		y *= vec.y;
		return *this;
	}

	constexpr Vec2 operator*=(const float s) {
		x *= s;
		y *= s;
		return *this;
	}

	constexpr Vec2 operator/=(const Vec2& vec) {
		x /= vec.x;
		y /= vec.y;
		return *this;
	}

	constexpr Vec2 operator/=(const float s) {
		x /= s;
		y /= s;
		return *this;
	}

	[[nodiscard]] float distance() const {
		return sqrt(x * x + y * y);
	}

	[[nodiscard]] float distanceto(const Vec2& vec) const {
		return sqrt((x - vec.x) * (x - vec.x) + (y - vec.y) * (y - vec.y));
	}

	[[nodiscard]] Vec2 normalize() const {
		return *this / distance();
	}

public:
	float x, y;
};