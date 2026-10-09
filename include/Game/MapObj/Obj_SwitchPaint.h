#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class SwitchPaintState : u32 {
    cOff = 0,
    cOn  = 1
};

/**
 * Obj_SwitchPaint / SwitchPaint / Obj_SwitchPaintVS
 * Address: vtable @ 0x100e4024
 * Ink-activated trigger switch that activates linked gates, lifts, and mechanisms
 * when coated with sufficient ink.
 *
 * Real PowerPC methods:
 *   vfunc_3  @ 0x025D94CC - Model & resource loading (Obj_SwitchPaint.szs)
 *   vfunc_5  @ 0x025D8600 - Initialization and signal link setup
 *   vfunc_7  @ 0x025D9894 - Main update tick & auto-reset countdown
 *   vfunc_11 @ 0x025D990C - Ink paint collision sensor
 *   vfunc_47 @ 0x025D8D70 - Indicator light & animation sync
 *   vfunc_55 @ 0x025D8670 - Signal broadcast to linked actors
 */
class Obj_SwitchPaint : public GambitActor {
public:
    static constexpr f32 cDefaultActivationThreshold = 10.0f;

    Obj_SwitchPaint();
    virtual ~Obj_SwitchPaint() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // PowerPC vtable slots matching Ghidra
    virtual void vfunc_3();
    virtual void vfunc_5();
    virtual void vfunc_7();
    virtual void vfunc_11();
    virtual void vfunc_47();
    virtual void vfunc_55();

    // Ink interaction
    bool paintInk(f32 inkAmount, u32 teamId = 0);
    void reset();

    // Configuration
    void setActivationThreshold(f32 threshold) { mActivationThreshold = threshold; }
    void setAutoResetFrames(s32 frames) { mAutoResetFrames = frames; }
    void setVsMode(bool enabled) { mIsVsMode = enabled; }

    // Status inspection
    SwitchPaintState getState() const { return mState; }
    bool isActivated() const { return mState == SwitchPaintState::cOn; }
    f32 getAccumulatedInk() const { return mAccumulatedInk; }
    f32 getActivationThreshold() const { return mActivationThreshold; }
    u32 getOwningTeam() const { return mOwningTeam; }
    bool isSignalSent() const { return mSignalSent; }
    bool consumeSignal() { bool s = mSignalSent; mSignalSent = false; return s; }
    const sead::Vector3f& getPosition() const { return mPosition; }

    void setPosition(const sead::Vector3f& pos) { mPosition = pos; }

protected:
    sead::Vector3f mPosition;      // 0x140

    SwitchPaintState mState;
    f32 mAccumulatedInk;
    f32 mActivationThreshold;
    s32 mAutoResetFrames;
    s32 mResetTimer;
    u32 mOwningTeam;
    bool mSignalSent;
    bool mIsVsMode;

    undefined mReserved[0x38];
};

// Internal binary aliases
using SwitchPaint       = Obj_SwitchPaint;
using Obj_SwitchPaintVS = Obj_SwitchPaint;
using SwitchPaintVS     = Obj_SwitchPaint;

} // namespace Game
