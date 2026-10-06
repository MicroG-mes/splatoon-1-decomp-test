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
};

using Matrix34f = Matrix34<f32>;
using Matrix44f = Matrix44<f32>;

} // namespace sead
