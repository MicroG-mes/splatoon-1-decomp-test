#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

class SoundShapeMgr : public GambitActor {
public:
    static constexpr f32 cDefaultMaxAudibleDist = 45.0f;

    SoundShapeMgr();
    virtual ~SoundShapeMgr() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setListenerTransform(const sead::Vector3f& listenerPos, const sead::Vector3f& forwardDir);
    void setInkImmersionFilter(bool isSubmergedInInk);
    bool calculateSpatialAudio(const sead::Vector3f& emitterPos, f32 maxDist, f32& outVolume, f32& outPan) const;
    void play3dSound(u32 soundId, const sead::Vector3f& emitterPos, f32 volume = 1.0f, f32 maxDist = cDefaultMaxAudibleDist);

    bool isSubmergedFilterActive() const { return mIsSubmergedFilterActive; }
    f32 getLowpassCutoffHz() const { return mLowpassCutoffHz; }
    const sead::Vector3f& getListenerPos() const { return mListenerPos; }

protected:
    sead::Vector3f mListenerPos;
    sead::Vector3f mListenerForward;
    bool mIsSubmergedFilterActive;
    f32 mLowpassCutoffHz;
    f32 mMasterVolume;

    undefined mReserved[0x38];
};

} // namespace Game
