#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class Obj_PlazaAmiibo : public GambitActor {
public:
    Obj_PlazaAmiibo();
    virtual ~Obj_PlazaAmiibo() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    bool checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius = 3.0f);
    void triggerOpenBox();
    void closeBox();

    bool isPromptActive() const { return mIsPlayerNearby; }
    bool isBoxOpen() const { return mIsOpen; }

protected:
    sead::Vector3f mPosition;
    f32 mInteractRadius;
    bool mIsPlayerNearby;
    bool mIsOpen;
    s32 mGlowAnimTimer;

    undefined mReserved[0x38];
};

} // namespace Game
