#pragma once

#include "types.h"
#include "sead/math/seadVector.h"

namespace sead {

template <typename T>
struct Matrix34 {
    T m[3][4];

    Matrix34() {
        makeIdentity();
    }

    void makeIdentity() {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }
    }
};

template <typename T>
struct Matrix44 {
    T m[4][4];

    Matrix44() {
        makeIdentity();
    }

    void makeIdentity() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }
    }

    void makeZero() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m[i][j] = static_cast<T>(0);
            }
        }
    }

    Matrix44 operator*(const Matrix44& o) const {
        Matrix44 res;
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                T sum = 0;
                for (int k = 0; k < 4; ++k) {
                    sum += m[r][k] * o.m[k][c];
                }
                res.m[r][c] = sum;
            }
        }
        return res;
    }

    Vector3<T> transformPoint(const Vector3<T>& p) const {
        T x = m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3];
        T y = m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3];
        T z = m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3];
        T w = m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3];
        if (std::abs(w) > static_cast<T>(1e-6)) {
            return Vector3<T>(x / w, y / w, z / w);
        }
        return Vector3<T>(x, y, z);
    }

    Vector3<T> transformVector(const Vector3<T>& v) const {
        T x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
        T y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
        T z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
        return Vector3<T>(x, y, z);
    }

    void setTranslation(T x, T y, T z) {
        makeIdentity();
        m[0][3] = x;
        m[1][3] = y;
        m[2][3] = z;
    }

    void setScale(T sx, T sy, T sz) {
        makeIdentity();
        m[0][0] = sx;
        m[1][1] = sy;
        m[2][2] = sz;
    }

    // DirectX 11 Look-At View Matrix (NDC depth [0, 1])
    void buildLookAtDX11(const Vector3<T>& eye, const Vector3<T>& at, const Vector3<T>& up) {
        makeIdentity();
        Vector3<T> zAxis = (at - eye).normalized();
        Vector3<T> xAxis = up.cross(zAxis).normalized();
        Vector3<T> yAxis = zAxis.cross(xAxis);

        m[0][0] = xAxis.x;
        m[0][1] = xAxis.y;
        m[0][2] = xAxis.z;
        m[0][3] = -xAxis.dot(eye);

        m[1][0] = yAxis.x;
        m[1][1] = yAxis.y;
        m[1][2] = yAxis.z;
        m[1][3] = -yAxis.dot(eye);

        m[2][0] = zAxis.x;
        m[2][1] = zAxis.y;
        m[2][2] = zAxis.z;
        m[2][3] = -zAxis.dot(eye);

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;
    }

    // DirectX 11 Perspective Projection Matrix (NDC depth [0, 1])
    void buildPerspectiveDX11(T fovYRad, T aspect, T zNear, T zFar) {
        makeZero();
        T tanHalfFov = std::tan(fovYRad / static_cast<T>(2));
        T yScale = static_cast<T>(1) / tanHalfFov;
        T xScale = yScale / aspect;
        T zRange = zFar - zNear;

        m[0][0] = xScale;
        m[1][1] = yScale;
        m[2][2] = zFar / zRange;
        m[2][3] = -(zNear * zFar) / zRange;
        m[3][2] = static_cast<T>(1);
        m[3][3] = static_cast<T>(0);
    }

    // DirectX 11 Orthographic Projection Matrix (NDC depth [0, 1])
    void buildOrthoDX11(T width, T height, T zNear, T zFar) {
        makeIdentity();
        m[0][0] = static_cast<T>(2) / width;
        m[1][1] = static_cast<T>(2) / height;
        m[2][2] = static_cast<T>(1) / (zFar - zNear);
        m[2][3] = -zNear / (zFar - zNear);
    }
};

using Matrix34f = Matrix34<f32>;
using Matrix44f = Matrix44<f32>;

} // namespace sead
