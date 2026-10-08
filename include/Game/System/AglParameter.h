#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Game {

/**
 * AglParameterObj
 * Authentic parser for Nintendo's agl::utl::ParameterObj text format (.params files).
 * Extensively used across Splatoon 1 for weapon tuning, gizmos, and ballistics.
 */
class AglParameterObj {
public:
    AglParameterObj();
    ~AglParameterObj();

    bool loadFromFile(const char* filePath);
    bool parseString(const std::string& text);

    bool hasKey(const char* key) const;
    f32 getFloat(const char* key, f32 defaultVal = 0.0f) const;
    s32 getInt(const char* key, s32 defaultVal = 0) const;
    std::string getString(const char* key, const char* defaultVal = "") const;
    std::vector<f32> getFloatArray(const char* key) const;

    size_t getParameterCount() const { return mEntries.size(); }

private:
    std::unordered_map<std::string, std::vector<std::string>> mEntries;
};

// Strongly-typed parameter structs

struct RollerWeaponParams {
    u32 swingLiftFrame;
    u32 splashNum;
    f32 splashInitSpeedBase;
    f32 splashInitSpeedRandomZ;
    f32 splashInitSpeedRandomX;
    f32 splashDeg;
    f32 moveSpeed;
    f32 inkConsumeSplash;

    bool load(const char* filePath);
};

struct ShieldParams {
    f32 maxHp;
    u32 preparationDurationFrame;
    u32 noDamageRunningDurationFrame;
    f32 damage;
    f32 boundVelLen;
    u32 paintRepeatFrame;
    f32 paintWidth;

    bool load(const char* filePath);
};

struct ShachihokoParams {
    f32 hp;
    f32 offsetY;
    u32 victoryPlayerTimeLimitFrame;
    f32 barrierRadius;
    f32 barrierMaxScale;

    bool load(const char* filePath);
};

struct TrapParams {
    f32 maxHp;
    f32 paintRadius;
    f32 playerColRadius;
    u32 timerFrame;
    u32 presageFrame;
    f32 bombCoreDamageNear;
    f32 bombCoreRadiusNear;
    f32 bombCoreDamageMiddle;
    f32 bombCoreRadiusMiddle;
    f32 bombCorePaintRadius;

    bool load(const char* filePath);
};

struct TurnPlateParams {
    f32 harfLen;
    f32 colZLen;
    f32 rotSpeed;
    f32 colHeight;
    f32 playerMoveSpeed;
    u32 lostTargetTime;
    u32 lockStartTime;

    bool load(const char* filePath);
};

} // namespace Game
