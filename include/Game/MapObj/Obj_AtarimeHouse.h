#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class AtarimeHouseState : u32 {
    cState_Normal   = 0,
    cState_Vacant   = 1,
    cState_InDanger = 2
};

/**
 * Obj_AtarimeHouse
 * Retail Address: vtable @ 0x1009bf80
 * Cap'n Cuttlefish's Cabin / Headquarters in Octo Valley overworld (Area 1).
 * Central base where Cuttlefish monitors Octarian zapfish thefts.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x0245a1c0 - Model loading (Obj_AtarimeHouse.szs, 29,147 vertices)
 *   vfunc_5  @ 0x0245b0a4 - Base compound boundary & chimney smoke anchor setup
 *   vfunc_7  @ 0x0245b630 - Environmental update (fabric antenna sway, wind dynamics)
 *   vfunc_14 @ 0x0245bf18 - Player compound proximity check
 */
class Obj_AtarimeHouse : public GambitActor {
public:
    static constexpr f32 cCompoundRadius = 12.0f;
    static constexpr f32 cChimneyOffsetY = 8.5f;

    Obj_AtarimeHouse();
    virtual ~Obj_AtarimeHouse() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_14();

    // Overworld mechanics
    bool isPlayerInCompound(const sead::Vector3f& playerPos) const;
    void setVacant(bool vacant);

    // Queries
    AtarimeHouseState getState() const { return mState; }
    f32 getAntennaSway() const { return mAntennaSway; }
    sead::Vector3f getChimneyPosition() const {
        return sead::Vector3f(mPosition.x, mPosition.y + cChimneyOffsetY, mPosition.z);
    }

    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140
    AtarimeHouseState mState;
    s32 mTimer;
    f32 mAntennaSway;

    undefined mReserved[0x38];
};

} // namespace Game
