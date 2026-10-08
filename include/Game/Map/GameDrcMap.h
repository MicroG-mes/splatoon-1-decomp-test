#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "Game/Map/MapTable.h"

namespace Game {

class PaintTextureMgr;

// GameDrcMap camera modes (recovered from 0x023F3D68 rodata @ 0x1009263C, 0x10092658)
enum class DrcMapCameraMode : u32 {
    cNeutralCamera   = 0, // s_NeutralCamera_10092658: Standard overview
    cArtilleryCamera = 1  // s_ArtilleryCamera_1009263c: Inkstrike targeting reticle
};

// GameDrcMap touch states (recovered from 0x023F3D68 rodata @ 0x10092634 - 0x10092684)
enum class DrcMapTouchState : u32 {
    cTouchOff      = 0, // s_TouchOff_10092668
    cTouchOn       = 1, // s_TouchOn_10092634
    cTouchOnSame   = 2, // s_TouchOnSame_1009264c
    cTouchDecided  = 3, // s_TouchDecided_10092674: Jump/Inkstrike confirmed
    cTouchInvalid  = 4  // s_TouchInvalid_10092684
};

struct DrcTouchPoint {
    f32 screenX; // 0.0f to 854.0f (Wii U DRC resolution is 854x480)
    f32 screenY; // 0.0f to 480.0f
    bool isTouched;
};

class GameDrcMap {
public:
    GameDrcMap();
    ~GameDrcMap();

    void init(const MapParam* stageParam, u32 localPlayerTeamId);
    void update();

    // Touch event handling (GamePad touch screen)
    void handleTouch(const DrcTouchPoint& touch);

    // Camera modes
    void setCameraMode(DrcMapCameraMode mode);
    DrcMapCameraMode getCameraMode() const { return mCameraMode; }

    // World to DRC screen mapping
    void worldToScreen(const sead::Vector3f& worldPos, f32& outScreenX, f32& outScreenY) const;
    void screenToWorld(f32 screenX, f32 screenY, sead::Vector3f& outWorldPos) const;

    // Team inversion: if local player is Team Bravo (1), map is rotated 180 degrees
    bool isBravoInverted() const { return mIsBravoInverted; }
    void setBravoInverted(bool inverted) { mIsBravoInverted = inverted; }

    DrcMapTouchState getTouchState() const { return mTouchState; }
    const sead::Vector3f& getTargetWorldPos() const { return mTargetWorldPos; }

    // Super Jump & Inkstrike actions
    bool hasTargetDecided() const { return mTouchState == DrcMapTouchState::cTouchDecided; }
    void consumeTargetDecision() { mTouchState = DrcMapTouchState::cTouchOff; }

private:
    const MapParam* mCurrentStage;
    u32 mLocalTeamId;
    bool mIsBravoInverted;
    DrcMapCameraMode mCameraMode;
    DrcMapTouchState mTouchState;

    sead::Vector3f mTargetWorldPos;
    f32 mCenterScreenX;
    f32 mCenterScreenY;
    f32 mZoomScale;
};

} // namespace Game
