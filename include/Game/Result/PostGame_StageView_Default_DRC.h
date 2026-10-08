#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "cafe/vpad.h"

namespace Game {

class PostGame_StageView_Default_DRC : public GambitActor {
public:
    PostGame_StageView_Default_DRC();
    virtual ~PostGame_StageView_Default_DRC() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void handleInput(const VPADStatus& vpad);

    void setCoverageResults(f32 alphaPercent, f32 bravoPercent);
    bool isContinueConfirmed() const { return mIsContinueConfirmed; }

protected:
    f32 mAlphaPercent;
    f32 mBravoPercent;
    bool mIsContinueConfirmed;
    s32 mAnimTimer;

    undefined mReserved[0x30];
};

} // namespace Game
