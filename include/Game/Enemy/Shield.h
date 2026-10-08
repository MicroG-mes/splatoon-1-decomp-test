#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class Shield : public GambitActor {
public:
    static constexpr f32 cShieldHalfWidth = 1.2f;
    static constexpr f32 cShieldHeight = 1.8f;
    static constexpr f32 cDeflectAngleRad = 1.309f; // +/- 75 degrees forward arc

    Shield();
    virtual ~Shield() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void attachToOwner(const sead::Vector3f& ownerPos, f32 facingYaw);
    bool checkFrontalDeflection(const sead::Vector3f& shotIncomingDir) const;

    f32 getFacingYaw() const { return mFacingYaw; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    f32 mFacingYaw;
    s32 mHitFlashTimer;

    undefined mReserved[0x38];
};

} // namespace Game
