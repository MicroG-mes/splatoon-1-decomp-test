#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class TakolienForm : u32 {
    cHumanoid = 0,
    cOctopus  = 1
};

enum class TakolienState : u32 {
    cPatrol         = 0,
    cEngageShooting = 1,
    cThrowBomb      = 2,
    cSwimDodge      = 3,
    cHitStagger     = 4,
    cDefeated       = 5
};

class Enm_Takolien : public GambitActor {
public:
    static constexpr f32 cMaxHp = 100.0f;
    static constexpr f32 cHumanRunSpeed = 0.08f;
    static constexpr f32 cOctoSwimSpeed = 0.16f;

    Enm_Takolien();
    virtual ~Enm_Takolien() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic Ghidra Decompiled vfuncs (from Enm_Takolien vtable 0x1008CED4)
    virtual void vfunc_7();  // 0x023B71EC: AI update tick & target evaluation
    virtual void vfunc_52(); // 0x023B73AC: Enter Humanoid form (stand & aim)
    virtual void vfunc_53(); // 0x023BC858: Enter Octopus form (ink swim & dodge)

    void updateAi(const sead::Vector3f& playerPos, bool isPlayerVisible);
    void applyDamage(f32 damage, const sead::Vector3f& hitDir);

    TakolienState getState() const { return mState; }
    TakolienForm getForm() const { return mForm; }
    f32 getHp() const { return mHp; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    void fireWeaponBurst(const sead::Vector3f& targetPos);
    void throwSplatBomb(const sead::Vector3f& targetPos);

    sead::Vector3f mPosition;
    sead::Vector3f mMoveDirection;
    f32 mHp;
    TakolienForm mForm;
    TakolienState mState;
    s32 mStateTimer;
    s32 mBurstShotCounter;

    // Authentic decompiled fields
    s32 mTargetAcquisitionFrames; // 0x1F0
    sead::Vector3f mOctoDodgePos; // 0x21C
    u8  mOctoFormFlags;           // 0x7C on sub-struct
};

} // namespace Game
