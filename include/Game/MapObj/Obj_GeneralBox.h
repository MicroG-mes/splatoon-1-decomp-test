#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "Game/Collision/KclFile.h"
#include "sead/math/seadVector.h"

namespace Game {

class Obj_GeneralBox : public GambitActor {
public:
    static constexpr f32 cDefaultSize = 2.4f;

    Obj_GeneralBox();
    virtual ~Obj_GeneralBox() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setup(const sead::Vector3f& position, f32 maxHp = 80.0f, f32 scale = 1.0f);
    bool loadCollision(const char* szsFilePath = "content/Model/Obj_GeneralBox.szs");

    bool checkBulletCollision(const sead::Vector3f& bulletPos, f32 radius, f32 damage, u8 teamId);
    void applyDamage(f32 damage, u8 teamId);

    bool isBroken() const { return mIsBroken; }
    f32 getRemainingHp() const { return mCurrentHp; }
    f32 getMaxHp() const { return mMaxHp; }
    u8 getCoveredTeam() const { return mCoveredTeam; }
    const sead::Vector3f& getPosition() const { return mPosition; }
    f32 getScale() const { return mScale; }

    const KclFile& getKclFile() const { return mKcl; }

private:
    sead::Vector3f mPosition;
    f32 mScale;
    f32 mMaxHp;
    f32 mCurrentHp;
    bool mIsBroken;
    s32 mBreakTimer;
    u8 mCoveredTeam;

    KclFile mKcl;
    bool mHasKcl;

    undefined mReserved[0x38];
};

} // namespace Game
