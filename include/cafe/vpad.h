#pragma once

#include "types.h"

// Cafe OS VPAD definitions for Wii U GamePad
enum VPADButtons : u32 {
    VPAD_BUTTON_A        = 0x8000,
    VPAD_BUTTON_B        = 0x4000,
    VPAD_BUTTON_X        = 0x2000,
    VPAD_BUTTON_Y        = 0x1000,
    VPAD_BUTTON_LEFT     = 0x0800,
    VPAD_BUTTON_RIGHT    = 0x0400,
    VPAD_BUTTON_UP       = 0x0200,
    VPAD_BUTTON_DOWN     = 0x0100,
    VPAD_BUTTON_ZL       = 0x0080,
    VPAD_BUTTON_ZR       = 0x0040,
    VPAD_BUTTON_L        = 0x0020,
    VPAD_BUTTON_R        = 0x0010,
    VPAD_BUTTON_PLUS     = 0x0008,
    VPAD_BUTTON_MINUS    = 0x0004,
    VPAD_BUTTON_HOME     = 0x0002,
    VPAD_BUTTON_SYNC     = 0x0001,
};

struct VPADStatus {
    u32 hold;
    u32 trigger;
    u32 release;
    struct {
        f32 x;
        f32 y;
    } lStick, rStick;
    struct {
        f32 x;
        f32 y;
        f32 z;
    } acc;
    struct {
        f32 x;
        f32 y;
        f32 z;
    } gyro;
    f32 angle[3];
    s8 error;
    u8 tpNormalCalibration;
    undefined padding[2];
};

extern "C" {
s32 VPADRead(s32 chan, VPADStatus* buffers, u32 count, s32* error);
}
