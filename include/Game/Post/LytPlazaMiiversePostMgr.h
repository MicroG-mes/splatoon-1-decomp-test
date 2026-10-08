#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "cafe/vpad.h"

namespace Game {

enum class PostMode : u32 {
    cClosed  = 0,
    cDrawing = 1,
    cConfirm = 2,
    cPosted  = 3
};

class LytPlazaMiiversePostMgr : public GambitActor {
public:
    LytPlazaMiiversePostMgr();
    virtual ~LytPlazaMiiversePostMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void openCanvas();
    void closeCanvas();
    void clearCanvas();

    void drawPixel(s32 x, s32 y, bool isBlack);
    bool getPixel(s32 x, s32 y) const;

    void addYeahLike();
    u32 getYeahCount() const { return mYeahCount; }

    PostMode getMode() const { return mMode; }
    bool isClosed() const { return mMode == PostMode::cClosed; }

protected:
    PostMode mMode;
    s32 mStateTimer;

    u8 mCanvasBitmap[4800]; // 320 * 120 / 8 bytes
    u32 mPenSize;           // 1, 2, or 4 px
    bool mIsEraser;
    u32 mYeahCount;

    undefined mReserved[0x38];
};

} // namespace Game
