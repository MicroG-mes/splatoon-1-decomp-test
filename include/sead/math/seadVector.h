#pragma once

#include "types.h"

namespace sead {

template <typename T>
struct Vector2 {
    T x;
    T y;

    Vector2() : x(0), y(0) {}
    Vector2(T x_, T y_) : x(x_), y(y_) {}
};

template <typename T>
struct Vector3 {
    T x;
    T y;
    T z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
};

template <typename T>
struct Vector4 {
    T x;
    T y;
    T z;
    T w;

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
};

using Vector2f = Vector2<f32>;
using Vector3f = Vector3<f32>;
using Vector4f = Vector4<f32>;

} // namespace sead
