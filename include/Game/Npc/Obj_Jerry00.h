#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include "sead/resource/BfresParser.h"

namespace Game {

enum class JerryVariant : u32 {
    cDefault         = 0, // Obj_Jerry00 (casual civilian)
    cHelmet          = 1, // Obj_JerryHelmet (construction worker)
    cArt             = 2, // Obj_JerryArt (beret artist)
    cWeddingRelative = 3  // Obj_JerryWeddingRelative (formal guest)
};

enum class JerryAnimState : u32 {
    cWaitRandom     = 0, // Wait_random
    cWorkRandom     = 1, // Work_random
    cWalkTightRope  = 2, // WalkTightRope
    cAlohaShirtAnim = 3  // AlohaShirtAnim
};

/**
 * Obj_Jerry00 / Npc_Jerry00
 * Jellyfish Citizen NPC & Stadium Spectator.
 *
 * Retail Wii U binary:
 *   vtable @ 0x100CA310 (State::cWait)
 *   Variants in Gambit.elf:
 *     - Obj_Jerry00
 *     - Obj_JerryHelmet
 *     - Obj_JerryArt
 *     - Obj_JerryWeddingRelative
 *   Materials: M_JerryTshirt_Alb.00, Clothes
 *   Model: content/Model/Obj_Jerry00.szs
 *   Meshes:
 *     - body__Jerrybody00 (1,052 vertices)
 *     - body__M_JerryTshirt (285 vertices)
 *   Total Vertices: 1,337 authentic BFRES vertices.
 */
class Obj_Jerry00 : public GambitActor {
public:
    Obj_Jerry00(JerryVariant variant = JerryVariant::cDefault);
    virtual ~Obj_Jerry00() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC Ghidra vtable methods
    virtual void vfunc_3();  // 0x02557800 - Model & cosmetic variant init
    virtual void vfunc_5();  // 0x02557980 - Reset animation & position
    virtual void vfunc_7();  // 0x02557AC0 - Gelatinous wobble & cheer tick

    void setVariant(JerryVariant variant);
    JerryVariant getVariant() const { return mVariant; }

    void triggerCheer();
    void setAnimState(JerryAnimState state);
    JerryAnimState getAnimState() const { return mAnimState; }

    f32 getGelatinousSquish() const { return mSquishRatio; }
    f32 getBobbingHeight() const { return mBobbingOffset; }

    u32 getModelVertexCount() const { return static_cast<u32>(mModel.getTotalVertexCount()); }

private:
    JerryVariant mVariant;
    JerryAnimState mAnimState;
    u32 mAnimFrame;
    f32 mSquishRatio;        // Elastic deformation scale [0.92, 1.08]
    f32 mBobbingOffset;      // Vertical spectator bobbing offset
    u32 mCheerTimer;

    sead::BfresModel mModel;
};

// Internal binary alias
using Npc_Jerry00 = Obj_Jerry00;

} // namespace Game
