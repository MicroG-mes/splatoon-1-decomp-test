#include "Game/System/PcInputBridge.h"
#include <cstring>
#include <cmath>
#include <algorithm>

namespace Game {

PcInputBridge::PcInputBridge()
    : mPrevHold(0)
{
    std::memset(&mCurrentStatus, 0, sizeof(mCurrentStatus));
}

PcInputBridge::~PcInputBridge() {
}

void PcInputBridge::init() {
    reset();
}

void PcInputBridge::reset() {
    mPrevHold = 0;
    std::memset(&mCurrentStatus, 0, sizeof(mCurrentStatus));
}

void PcInputBridge::update(const PcRawInputState& raw, VPADStatus* outStatus) {
    u32 currentHold = 0;

    // Movement buttons / actions
    if (raw.keySpace)    currentHold |= VPAD_BUTTON_B;    // Space = Jump (B)
    if (raw.mouseLeft)   currentHold |= VPAD_BUTTON_ZR;   // Left Click = Fire Main Weapon (ZR)
    if (raw.mouseRight)  currentHold |= VPAD_BUTTON_R;    // Right Click = Sub Weapon (R)
    if (raw.keyShift)    currentHold |= VPAD_BUTTON_ZL;   // Shift = Squid Form / Submerge (ZL)
    if (raw.keyE)        currentHold |= VPAD_BUTTON_X;    // E = Special / Super Jump Map (X)
    if (raw.keyR)        currentHold |= VPAD_BUTTON_Y;    // R = Reset Camera (Y)
    if (raw.keyTab)      currentHold |= VPAD_BUTTON_PLUS; // Tab = Map / Scoreboard (PLUS)

    mCurrentStatus.trigger = (currentHold ^ mPrevHold) & currentHold;
    mCurrentStatus.release = (currentHold ^ mPrevHold) & mPrevHold;
    mCurrentStatus.hold    = currentHold;
    mPrevHold = currentHold;

    // Left Analog Stick (WASD Movement)
    f32 stickX = (raw.keyD ? 1.0f : 0.0f) - (raw.keyA ? 1.0f : 0.0f);
    f32 stickY = (raw.keyW ? 1.0f : 0.0f) - (raw.keyS ? 1.0f : 0.0f);

    // Normalize diagonal velocity vector to prevent diagonal speed boost
    f32 stickLenSq = stickX * stickX + stickY * stickY;
    if (stickLenSq > 1.0f) {
        f32 invLen = 1.0f / std::sqrt(stickLenSq);
        stickX *= invLen;
        stickY *= invLen;
    }

    mCurrentStatus.leftStick.x = stickX;
    mCurrentStatus.leftStick.y = stickY;

    // Right Analog Stick & Gyroscope (Mouse Look Delta)
    f32 sens = raw.mouseSensitivity > 0.0f ? raw.mouseSensitivity : 0.02f;
    f32 lookX = std::max(-1.0f, std::min(1.0f, raw.mouseDeltaX * sens));
    f32 lookY = std::max(-1.0f, std::min(1.0f, -raw.mouseDeltaY * sens));

    mCurrentStatus.rightStick.x = lookX;
    mCurrentStatus.rightStick.y = lookY;

    // Wii U Gyro emulation
    mCurrentStatus.gyro.x = raw.mouseDeltaY * sens * 5.0f;
    mCurrentStatus.gyro.y = raw.mouseDeltaX * sens * 5.0f;
    mCurrentStatus.gyro.z = 0.0f;

    if (outStatus) {
        *outStatus = mCurrentStatus;
    }
}

} // namespace Game
