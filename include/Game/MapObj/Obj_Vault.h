#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class VaultState : u32 {
    cLocked    = 0,
    cUnlocking = 1,
    cOpened    = 2
};

/**
 * Obj_Vault
 * Chained treasure chest vault in Octo Valley single-player campaign.
 * Bound by padlocked chains that shatter when the player presents the matching Card Key.
 */
class Obj_Vault : public GambitActor {
public:
    Obj_Vault();
    virtual ~Obj_Vault() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupVault(u32 keyId, const sead::Vector3f& pos);
    bool tryUnlockWithKey(u32 keyId);

    VaultState getState() const { return mState; }
    u32 getKeyId() const { return mRequiredKeyId; }
    bool isOpened() const { return mState == VaultState::cOpened; }
    f32 getLidOpenAngle() const { return mLidAngle; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    u32 mRequiredKeyId;
    VaultState mState;
    f32 mLidAngle;
    s32 mTimer;
};

} // namespace Game
