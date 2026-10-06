#pragma once

#include "types.h"
#include "sead/math/seadVector.h"

namespace Game {

struct PlayerBehindCamera {
    undefined field0_0x0[0x10];
    sead::Vector3f mCameraPos;
    sead::Vector3f mCameraTarget;
    f32 mFov;
    undefined field_unk[0x50];

    virtual ~PlayerBehindCamera();
    virtual void update();
};

} // namespace Game
