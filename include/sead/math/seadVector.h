#pragma once

#include "types.h"

#include <cmath>

namespace sead {

template <typename T>
struct Vector2 {
    T x;
    T y;

    Vector2() : x(0), y(0) {}
    Vector2(T x_, T y_) : x(x_), y(y_) {}
    void set(T x_, T y_) { x = x_; y = y_; }

    Vector2 operator+(const Vector2& o) const { return Vector2(x + o.x, y + o.y); }
    Vector2 operator-(const Vector2& o) const { return Vector2(x - o.x, y - o.y); }
    Vector2 operator*(T s) const { return Vector2(x * s, y * s); }
    Vector2 operator/(T s) const { return Vector2(x / s, y / s); }
    T dot(const Vector2& o) const { return x * o.x + y * o.y; }
    T lengthSq() const { return x * x + y * y; }
    T length() const { return std::sqrt(lengthSq()); }
};

template <typename T>
struct Vector3 {
    T x;
    T y;
    T z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
    void set(T x_, T y_, T z_) { x = x_; y = y_; z = z_; }

    Vector3 operator+(const Vector3& o) const { return Vector3(x + o.x, y + o.y, z + o.z); }
    Vector3 operator-(const Vector3& o) const { return Vector3(x - o.x, y - o.y, z - o.z); }
    Vector3 operator*(T s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator/(T s) const { return Vector3(x / s, y / s, z / s); }
    Vector3& operator+=(const Vector3& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vector3& operator-=(const Vector3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vector3& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }

    T dot(const Vector3& o) const { return x * o.x + y * o.y + z * o.z; }
    Vector3 cross(const Vector3& o) const {
        return Vector3(
            y * o.z - z * o.y,
            z * o.x - x * o.z,
            x * o.y - y * o.x
        );
    }
    T lengthSq() const { return x * x + y * y + z * z; }
    T length() const { return std::sqrt(lengthSq()); }
    Vector3 normalized() const {
        T len = length();
        if (len > static_cast<T>(1e-6)) {
            return *this * (static_cast<T>(1) / len);
        }
        return Vector3(0, 0, 0);
    }
    void normalize() {
        *this = normalized();
    }
};

template <typename T>
struct Vector4 {
    T x;
    T y;
    T z;
    T w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
    void set(T x_, T y_, T z_, T w_) { x = x_; y = y_; z = z_; w = w_; }
    Vector4 operator+(const Vector4& o) const { return Vector4(x + o.x, y + o.y, z + o.z, w + o.w); }
    Vector4 operator*(T s) const { return Vector4(x * s, y * s, z * s, w * s); }
    T dot(const Vector4& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }
};

using Vector2f = Vector2<f32>;
using Vector3f = Vector3<f32>;
using Vector4f = Vector4<f32>;

} // namespace sead
