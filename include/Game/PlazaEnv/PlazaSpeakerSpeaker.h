#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class PlazaSpeakerSpeaker : public GambitActor {
public:
    PlazaSpeakerSpeaker();
    virtual ~PlazaSpeakerSpeaker() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setBpm(f32 bpm);
    f32 getSpeakerPulseScale() const { return mPulseScale; }

protected:
    f32 mBpm;
    f32 mBeatPhase;
    f32 mPulseScale;
    s32 mAnimTimer;

    undefined mReserved[0x38];
};

} // namespace Game
