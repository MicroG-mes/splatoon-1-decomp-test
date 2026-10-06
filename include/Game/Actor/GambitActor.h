#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "sead/math/seadMatrix.h"
#include "sead/heap/seadHeap.h"

namespace Game {

class GambitActor {
public:
    GambitActor();
    virtual ~GambitActor();

    // Core Actor Lifecycle virtual methods (from vtable @ 0x100000E4)
    virtual void init();
    virtual void update();
    virtual void postUpdate();
    virtual void draw();
    virtual void destroy();

    // Spatial & Transform
    const sead::Vector3f& getPosition() const { return mPosition; }
    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

    const sead::Vector3f& getRotation() const { return mRotation; }
    void setRotation(const sead::Vector3f& rot) { mRotation = rot; }

    // Child Heap creation for Actor resources
    virtual sead::Heap* createActorLoadChildHeap(size_t heapSize, const char* heapName);

    // HIO Debugging Callbacks (recovered from rodata table @ 0x02D4E638)
    virtual void openHIO();
    virtual void moveToActor();
    virtual void onDeathOrComplete();

protected:
    sead::Vector3f mPosition;
    sead::Vector3f mRotation;
    sead::Vector3f mScale;
    sead::Heap* mChildHeap;
    u32 mActorId;
    u32 mActorFlags;
    undefined mReserved[0x40];
};

} // namespace Game
