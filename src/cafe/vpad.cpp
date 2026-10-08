#include "cafe/vpad.h"
#include <windows.h>
#include <cmath>

static u32 sPrevHold = 0;
static POINT sPrevCursorPos = {0, 0};
static bool sHasPrevCursor = false;

extern "C" {

s32 VPADRead(s32 chan, VPADStatus* buffers, u32 count, s32* error) {
    if (!buffers || count == 0) {
        if (error) *error = -1;
        return 0;
    }

    VPADStatus& status = buffers[0];
    status.hold = 0;
    status.trigger = 0;
    status.release = 0;
    status.lStick.x = 0.0f;
    status.lStick.y = 0.0f;
    status.rStick.x = 0.0f;
    status.rStick.y = 0.0f;
    status.gyro.x = 0.0f;
    status.gyro.y = 0.0f;
    status.gyro.z = 0.0f;
    status.tpData.touched = 0;
    status.tpData.validity = 0;
    status.error = 0;

    // Check keyboard keys via GetAsyncKeyState
    // Movement: WASD -> GamePad Left Stick
    if (GetAsyncKeyState('W') & 0x8000) status.lStick.y += 1.0f;
    if (GetAsyncKeyState('S') & 0x8000) status.lStick.y -= 1.0f;
    if (GetAsyncKeyState('D') & 0x8000) status.lStick.x += 1.0f;
    if (GetAsyncKeyState('A') & 0x8000) status.lStick.x -= 1.0f;

    // Normalize left stick if diagonal
    f32 lMag = std::sqrt(status.lStick.x * status.lStick.x + status.lStick.y * status.lStick.y);
    if (lMag > 1.0f) {
        status.lStick.x /= lMag;
        status.lStick.y /= lMag;
    }

    // Buttons:
    // Space -> B (Jump)
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        status.hold |= VPAD_BUTTON_B;
    }

    // Left Mouse Button -> ZR (Shoot main weapon)
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
        status.hold |= VPAD_BUTTON_ZR;
    }

    // Right Mouse Button -> ZL (Squid dive)
    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
        status.hold |= VPAD_BUTTON_ZL;
    }

    // Middle Mouse or Q -> R (Sub-weapon throw)
    if ((GetAsyncKeyState(VK_MBUTTON) & 0x8000) || (GetAsyncKeyState('Q') & 0x8000)) {
        status.hold |= VPAD_BUTTON_R;
    }

    // E or F -> Special activation (Right Stick Press)
    if ((GetAsyncKeyState('E') & 0x8000) || (GetAsyncKeyState('F') & 0x8000)) {
        status.hold |= VPAD_BUTTON_PLUS;
    }

    // X -> Open Super Jump map
    if (GetAsyncKeyState('X') & 0x8000) {
        status.hold |= VPAD_BUTTON_X;
    }

    // Y -> Camera reset
    if (GetAsyncKeyState('Y') & 0x8000) {
        status.hold |= VPAD_BUTTON_Y;
    }

    // Mouse delta -> Gyro Motion Aiming + Right Stick Camera Look
    POINT curPos;
    if (GetCursorPos(&curPos)) {
        if (sHasPrevCursor) {
            f32 dx = static_cast<f32>(curPos.x - sPrevCursorPos.x);
            f32 dy = static_cast<f32>(curPos.y - sPrevCursorPos.y);

            // Gyro angular rates (yaw and pitch)
            status.gyro.y = -dx * 0.05f;
            status.gyro.x = -dy * 0.05f;

            // Camera right stick analogue
            status.rStick.x = dx * 0.02f;
            status.rStick.y = -dy * 0.02f;
            if (status.rStick.x > 1.0f) status.rStick.x = 1.0f;
            if (status.rStick.x < -1.0f) status.rStick.x = -1.0f;
            if (status.rStick.y > 1.0f) status.rStick.y = 1.0f;
            if (status.rStick.y < -1.0f) status.rStick.y = -1.0f;
        }
        sPrevCursorPos = curPos;
        sHasPrevCursor = true;
    }

    // Compute edge triggers & releases
    status.trigger = status.hold & (~sPrevHold);
    status.release = sPrevHold & (~status.hold);
    sPrevHold = status.hold;

    if (error) *error = 0;
    return 1;
}

} // extern "C"
