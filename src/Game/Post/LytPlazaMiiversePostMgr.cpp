#include "Game/Post/LytPlazaMiiversePostMgr.h"
#include <cstring>

namespace Game {

LytPlazaMiiversePostMgr::LytPlazaMiiversePostMgr()
    : mMode(PostMode::cClosed),
      mStateTimer(0),
      mPenSize(2),
      mIsEraser(false),
      mYeahCount(0) {
    std::memset(mCanvasBitmap, 0, sizeof(mCanvasBitmap));
}

LytPlazaMiiversePostMgr::~LytPlazaMiiversePostMgr() {
}

void LytPlazaMiiversePostMgr::init() {
    GambitActor::init();
    mMode = PostMode::cClosed;
    mYeahCount = 0;
}

void LytPlazaMiiversePostMgr::openCanvas() {
    mMode = PostMode::cDrawing;
    mStateTimer = 0;
}

void LytPlazaMiiversePostMgr::closeCanvas() {
    mMode = PostMode::cClosed;
}

void LytPlazaMiiversePostMgr::clearCanvas() {
    std::memset(mCanvasBitmap, 0, sizeof(mCanvasBitmap));
}

void LytPlazaMiiversePostMgr::drawPixel(s32 x, s32 y, bool isBlack) {
    if (x < 0 || x >= 320 || y < 0 || y >= 120) {
        return;
    }

    s32 bitIndex = y * 320 + x;
    s32 byteIndex = bitIndex / 8;
    s32 bitOffset = bitIndex % 8;

    if (isBlack) {
        mCanvasBitmap[byteIndex] |= (1 << bitOffset);
    } else {
        mCanvasBitmap[byteIndex] &= ~(1 << bitOffset);
    }
}

bool LytPlazaMiiversePostMgr::getPixel(s32 x, s32 y) const {
    if (x < 0 || x >= 320 || y < 0 || y >= 120) {
        return false;
    }

    s32 bitIndex = y * 320 + x;
    s32 byteIndex = bitIndex / 8;
    s32 bitOffset = bitIndex % 8;

    return (mCanvasBitmap[byteIndex] & (1 << bitOffset)) != 0;
}

void LytPlazaMiiversePostMgr::addYeahLike() {
    mYeahCount++;
}

void LytPlazaMiiversePostMgr::handleInput(const VPADStatus& vpad) {
    switch (mMode) {
        case PostMode::cDrawing: {
            if (vpad.tpData.touched) {
                // Map GamePad touch coordinates (0..1280, 0..720) to canvas (0..320, 0..120)
                s32 cx = (vpad.tpData.x * 320) / 1280;
                s32 cy = (vpad.tpData.y * 120) / 720;
                drawPixel(cx, cy, !mIsEraser);
            }

            if (vpad.trigger & VPAD_BUTTON_X) {
                mIsEraser = !mIsEraser; // Toggle pen / eraser
            } else if (vpad.trigger & VPAD_BUTTON_Y) {
                clearCanvas();
            } else if (vpad.trigger & VPAD_BUTTON_PLUS) {
                mMode = PostMode::cConfirm;
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                closeCanvas();
            }
            break;
        }

        case PostMode::cConfirm: {
            if (vpad.trigger & VPAD_BUTTON_A) {
                mMode = PostMode::cPosted;
                mStateTimer = 0;
            } else if (vpad.trigger & VPAD_BUTTON_B) {
                mMode = PostMode::cDrawing;
            }
            break;
        }

        case PostMode::cPosted: {
            if (mStateTimer > 60) {
                closeCanvas();
            }
            break;
        }

        default:
            break;
    }
}

void LytPlazaMiiversePostMgr::update() {
    mStateTimer++;
}

void LytPlazaMiiversePostMgr::draw() {
    GambitActor::draw();
}

} // namespace Game
