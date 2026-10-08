#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include <vector>
#include <mutex>
#include <atomic>

namespace Game {

enum SplatoonSoundId : u32 {
    cSoundId_Shoot_Splattershot = 1001,
    cSoundId_Hit_Confirm        = 1002,
    cSoundId_Squid_Dive         = 1003,
    cSoundId_Squid_Swim         = 1004,
    cSoundId_Splat_Bomb_Throw   = 1005,
    cSoundId_Splat_Bomb_Explode = 1006,
    cSoundId_Super_Jump         = 1007,
    cSoundId_Crate_Break        = 1008,
    cSoundId_Low_Ink_Warning    = 1009,
    cSoundId_Roller_Fling       = 1010,
    cSoundId_Charger_Fire       = 1011,
    cSoundId_Charger_Charge     = 1012,
    cSoundId_Octoling_Splat     = 1013
};

struct ActiveVoice {
    u32 soundId;
    f32 volume;
    f32 pan;          // -1.0 (Left) to +1.0 (Right)
    bool submerged;   // Lowpass ink filter
    u32 sampleIndex;
    u32 totalSamples;
    f32 phase;
    f32 lpfState[2];
};

class PcAudioDriver {
public:
    static PcAudioDriver& instance();

    bool init();
    void shutdown();
    bool isAvailable() const { return mInitialized && mAudioDeviceOpened; }

    void playSound(u32 soundId, f32 volume = 1.0f, f32 pan = 0.0f, bool submerged = false);
    void play3dSound(u32 soundId, const sead::Vector3f& emitterPos, const sead::Vector3f& listenerPos,
                     const sead::Vector3f& listenerForward, f32 maxDist = 45.0f, f32 volume = 1.0f,
                     bool submerged = false);

    size_t getActiveVoiceCount();

private:
    PcAudioDriver();
    ~PcAudioDriver();
    PcAudioDriver(const PcAudioDriver&) = delete;
    PcAudioDriver& operator=(const PcAudioDriver&) = delete;

    void audioThreadLoop();
    void generateAudioBlock(s16* outBuffer, u32 numFrames);

    std::atomic<bool> mInitialized{false};
    std::atomic<bool> mAudioDeviceOpened{false};
    std::atomic<bool> mRunning{false};

    std::vector<ActiveVoice> mVoices;
    std::mutex mVoiceMutex;

    void* mWaveOutHandle{nullptr};
    void* mThreadHandle{nullptr};
};

} // namespace Game
