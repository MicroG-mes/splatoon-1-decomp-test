#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"

namespace Game {

class RegularRule : public GambitActor {
public:
    RegularRule();
    virtual ~RegularRule() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void updatePaintCoverage(u32 alphaInkedPixels, u32 bravoInkedPixels, u32 totalPaintablePixels);

    f32 getAlphaPercent() const { return mAlphaPercent; }
    f32 getBravoPercent() const { return mBravoPercent; }
    f32 getNeutralPercent() const { return mNeutralPercent; }

    bool didAlphaWin() const;

protected:
    f32 mAlphaPercent;
    f32 mBravoPercent;
    f32 mNeutralPercent;

    u32 mAlphaPoints;
    u32 mBravoPoints;

    undefined mReserved[0x30];
};

} // namespace Game
