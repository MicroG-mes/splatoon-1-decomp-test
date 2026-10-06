#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/container/seadTList.h"
#include "sead/heap/seadHeap.h"

namespace Game {

class GambitActorMgr {
public:
    GambitActorMgr();
    virtual ~GambitActorMgr();

    // Singleton access
    static GambitActorMgr* instance() { return sInstance; }

    // Core Manager Lifecycle (from vtable @ 0x100003BC)
    virtual void init(sead::Heap* heap);
    virtual void update();
    virtual void postUpdate();
    virtual void draw();
    virtual void finalize();

    // Actor Registration & Management
    void registerActor(GambitActor* actor);
    void unregisterActor(GambitActor* actor);

    // Queries
    GambitActor* findActorById(u32 actorId) const;
    s32 getActorCount() const { return mActorList.size(); }

    static GambitActorMgr* sInstance;

protected:
    sead::TList<GambitActor*> mActorList;
    sead::Heap* mHeap;
    u32 mNextActorId;
    undefined mReserved[0x30];
};

} // namespace Game
