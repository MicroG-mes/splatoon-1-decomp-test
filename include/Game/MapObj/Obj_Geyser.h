#pragma once

#include "types.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class GeyserState : u32 {
    cState_Closed = 0,
    cState_Opening,
    cState_Erupting,
    cState_Closing
};

struct GeyserParams {
    f32 hp;                         // mHp (0.30)
    f32 playerBindVel;              // mPlayerBindVel (2.50)
    f32 playerBindLerpRate;         // mPlayerBindLerpRate (0.05)
    f32 bombCorePosOffsetY;         // mBombCorePosOffsetY (5.0)
    f32 bombCorePaintRadius;        // mBombCorePaintRadius (45.0)
    f32 bombCoreDamageRadiusNear;   // mBombCoreDamageRadiusNear (40.0)
    f32 bombCoreDamageNear;         // mBombCoreDamageNear (0.80)
    f32 targetRadius;               // mTargetRadius (15.0)
    f32 fountainHeight;             // mUmbrellaColOffHeight (30.0)

    bool load(const char* paramsPath);
};

/**
 * Obj_Geyser
 * Authentic Splatoon 1 Ink Geyser (Gusher) mechanism.
 * Inking the closed valve triggers an ink eruption providing high vertical mobility.
 */
class Obj_Geyser {
public:
    static constexpr s32 cDefaultEruptDuration = 600; // 10 seconds at 60 fps

    Obj_Geyser();
    ~Obj_Geyser();

    void init(const sead::Vector3f& pos, u32 teamId = 0);
    void update();

    // Damage intake
    bool applyInkDamage(f32 damage, u32 teamId);

    // Swimming interaction: binds player to vertical ascent column
    bool updatePlayerSwimAscent(sead::Vector3f& playerPos, f32& outVerticalVel) const;

    // Getters
    GeyserState getState() const { return mState; }
    f32 getCurrentHeight() const { return mCurrentHeight; }
    f32 getMaxHeight() const { return mParams.fountainHeight; }
    u32 getTeamId() const { return mTeamId; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    const GeyserParams& getParams() const { return mParams; }
    bool isErupting() const { return mState == GeyserState::cState_Erupting; }

private:
    sead::Vector3f mPosition;
    GeyserState mState;
    GeyserParams mParams;
    u32 mTeamId;
    f32 mCurrentDamage;
    f32 mCurrentHeight;
    s32 mEruptTimer;
};

} // namespace Game
