#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class OctLegState : u32 {
    cState_Idle       = 0,
    cState_Appear     = 1,
    cState_Sweep      = 2,
    cState_Slam       = 3,
    cState_Shoot      = 4,
    cState_Damage     = 5,
    cState_ArmorBreak = 6,
    cState_Dazed      = 7,
    cState_Die        = 8
};

/**
 * Enm_OctLeg / EnemyOctLeg
 * Retail Address: vtable @ 0x1007d630
 * Octo Valley giant boss tentacle extremity and segmented shockwave striker.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x023657dc - Model & resource loading (Enm_OctLeg.szs, Enm_Break00/01/02)
 *   vfunc_5  @ 0x02365aa0 - Parameter initialization from Enm_OctLeg.params
 *   vfunc_7  @ 0x023666e4 - Segmented tentacle motion update (body4..body7) & shock wave
 *   vfunc_11 @ 0x0236672c - BulletEnemyBubbleShotOctLeg projectile emitter
 *   vfunc_47 @ 0x0236aee0 - EnemyParametersMgr parameter sync
 *   vfunc_52 @ 0x02368a54 - Damage handling & frontal armor deflection
 *   vfunc_60 @ 0x02368d7c - Death splat (20m radius) & broken armor piece burst
 */
class Enm_OctLeg : public GambitActor {
public:
    // Retail parameters from Enm_OctLeg.params
    static constexpr f32 cBaseLife                = 180.0f; // mLife: 1.8 (or 180 HP in player damage units)
    static constexpr f32 cEyesightRadius          = 300.0f;
    static constexpr f32 cEyesightViewAngle       = 235.0f;
    static constexpr f32 cDiePaintRadius          = 20.0f;
    static constexpr f32 cShockWaveRadius         = 160.0f;
    static constexpr f32 cShockWaveCoreImpact     = 1.25f;
    static constexpr f32 cShockWaveDamage         = 0.10f;
    static constexpr f32 cShockWaveRadius_Protect = 45.0f;
    static constexpr f32 cShieldOffsetZ           = 8.0f;
    static constexpr f32 cShieldRadius            = 10.0f;
    static constexpr f32 cShotSpeed               = 1.50f;
    static constexpr s32 cShotWaitFrame           = 180;
    static constexpr s32 cLostTargetTime          = 105;
    static constexpr s32 cBombChaseFrame          = 240;
    static constexpr f32 cArmorBreakThresholdHp   = 90.0f;

    Enm_OctLeg();
    virtual ~Enm_OctLeg() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_47();
    virtual void vfunc_52();
    virtual void vfunc_60();

    // Combat & tentacle mechanics
    void appear(const sead::Vector3f& pos);
    bool checkSight(const sead::Vector3f& targetPos) const;
    void triggerSlam();
    bool tryShootBubble();
    bool receiveDamage(f32 damage, const sead::Vector3f& hitPos, const sead::Vector3f& hitDir, bool& outShieldDeflected);

    // Queries
    OctLegState getState() const { return mState; }
    f32 getHealth() const { return mHealth; }
    bool hasArmor() const { return mHasArmor; }
    bool isShockWaveActive() const { return mShockWaveActive; }
    f32 getShockWaveProgress() const { return mShockWaveRadiusProgress; }
    s32 getShotCooldown() const { return mShotTimer; }
    bool isDefeated() const { return mState == OctLegState::cState_Die; }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

    // Segmented tentacle joint positions (body4..body7)
    const sead::Vector3f& getSegmentPos(size_t index) const {
        return mSegmentPositions[index < 4 ? index : 3];
    }

protected:
    sead::Vector3f mPosition;      // 0x140
    sead::Vector3f mSegmentPositions[4]; // body4, body5, body6, body7

    OctLegState mState;
    f32 mHealth;
    f32 mShockWaveRadiusProgress;
    s32 mStateTimer;
    s32 mShotTimer;
    bool mHasArmor;
    bool mShockWaveActive;

    undefined mReserved[0x38];
};

using EnemyOctLeg = Enm_OctLeg;

} // namespace Game
