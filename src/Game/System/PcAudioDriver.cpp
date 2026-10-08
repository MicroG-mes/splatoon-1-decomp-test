#include "Game/System/PcAudioDriver.h"
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <chrono>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

namespace Game {

static constexpr u32 cSampleRate = 44100;
static constexpr u32 cBufferSize = 1024; // ~23ms latency per block
static constexpr f32 cTwoPi = 6.28318530717958647692f;

PcAudioDriver& PcAudioDriver::instance() {
    static PcAudioDriver sInstance;
    return sInstance;
}

PcAudioDriver::PcAudioDriver() {
}

PcAudioDriver::~PcAudioDriver() {
    shutdown();
}

bool PcAudioDriver::init() {
    if (mInitialized.load()) return true;

#ifdef _WIN32
    WAVEFORMATEX wfx;
    ZeroMemory(&wfx, sizeof(wfx));
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 2;
    wfx.nSamplesPerSec = cSampleRate;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    wfx.cbSize = 0;

    HWAVEOUT hWaveOut = NULL;
    MMRESULT res = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    if (res != MMSYSERR_NOERROR) {
        // Fallback: system has no audio device or is headless
        mAudioDeviceOpened = false;
        mInitialized = true;
        return false;
    }

    mWaveOutHandle = static_cast<void*>(hWaveOut);
    mAudioDeviceOpened = true;
    mRunning = true;
    mInitialized = true;

    // Start background mixer thread
    std::thread audioWorker(&PcAudioDriver::audioThreadLoop, this);
    audioWorker.detach();
    return true;
#else
    mAudioDeviceOpened = false;
    mInitialized = true;
    return false;
#endif
}

void PcAudioDriver::shutdown() {
    if (!mInitialized.load()) return;

    mRunning = false;
#ifdef _WIN32
    if (mWaveOutHandle) {
        HWAVEOUT hWave = static_cast<HWAVEOUT>(mWaveOutHandle);
        waveOutReset(hWave);
        waveOutClose(hWave);
        mWaveOutHandle = nullptr;
    }
#endif
    std::lock_guard<std::mutex> lock(mVoiceMutex);
    mVoices.clear();
    mAudioDeviceOpened = false;
    mInitialized = false;
}

void PcAudioDriver::playSound(u32 soundId, f32 volume, f32 pan, bool submerged) {
    if (!mAudioDeviceOpened.load()) return;

    ActiveVoice v;
    v.soundId = soundId;
    v.volume = std::max(0.0f, std::min(volume, 1.0f));
    v.pan = std::max(-1.0f, std::min(pan, 1.0f));
    v.submerged = submerged;
    v.sampleIndex = 0;
    v.phase = 0.0f;
    v.lpfState[0] = 0.0f;
    v.lpfState[1] = 0.0f;

    switch (soundId) {
    case cSoundId_Shoot_Splattershot:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.12f); // 120ms
        break;
    case cSoundId_Hit_Confirm:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.10f); // 100ms
        break;
    case cSoundId_Squid_Dive:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.22f); // 220ms
        break;
    case cSoundId_Squid_Swim:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.08f); // 80ms
        break;
    case cSoundId_Splat_Bomb_Throw:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.25f); // 250ms
        break;
    case cSoundId_Splat_Bomb_Explode:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.60f); // 600ms
        break;
    case cSoundId_Super_Jump:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.70f); // 700ms
        break;
    case cSoundId_Crate_Break:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.35f); // 350ms
        break;
    case cSoundId_Low_Ink_Warning:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.14f); // 140ms
        break;
    case cSoundId_Roller_Fling:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.20f); // 200ms
        break;
    case cSoundId_Charger_Charge:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.10f); // 100ms
        break;
    case cSoundId_Charger_Fire:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.22f); // 220ms
        break;
    case cSoundId_Octoling_Splat:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.45f); // 450ms
        break;
    default:
        v.totalSamples = static_cast<u32>(cSampleRate * 0.15f);
        break;
    }

    std::lock_guard<std::mutex> lock(mVoiceMutex);
    // Limit max polyphony to 32 concurrent voices
    if (mVoices.size() < 32) {
        mVoices.push_back(v);
    }
}

void PcAudioDriver::play3dSound(u32 soundId, const sead::Vector3f& emitterPos,
                               const sead::Vector3f& listenerPos,
                               const sead::Vector3f& listenerForward,
                               f32 maxDist, f32 volume, bool submerged) {
    if (!mAudioDeviceOpened.load()) return;

    f32 dx = emitterPos.x - listenerPos.x;
    f32 dy = emitterPos.y - listenerPos.y;
    f32 dz = emitterPos.z - listenerPos.z;
    f32 dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist >= maxDist) return;

    f32 atten = 1.0f - (dist / maxDist);
    f32 finalVol = volume * atten;

    // Stereo panning
    f32 rightX =  listenerForward.z;
    f32 rightZ = -listenerForward.x;
    f32 pan = (dx * rightX + dz * rightZ) / (dist > 0.001f ? dist : 1.0f);

    playSound(soundId, finalVol, pan, submerged);
}

size_t PcAudioDriver::getActiveVoiceCount() {
    std::lock_guard<std::mutex> lock(mVoiceMutex);
    return mVoices.size();
}

void PcAudioDriver::generateAudioBlock(s16* outBuffer, u32 numFrames) {
    std::vector<f32> mixL(numFrames, 0.0f);
    std::vector<f32> mixR(numFrames, 0.0f);

    {
        std::lock_guard<std::mutex> lock(mVoiceMutex);

        for (auto it = mVoices.begin(); it != mVoices.end();) {
            ActiveVoice& v = *it;

            f32 leftGain = std::max(0.0f, (1.0f - v.pan) * 0.5f) * v.volume;
            f32 rightGain = std::max(0.0f, (1.0f + v.pan) * 0.5f) * v.volume;
            f32 alphaLpf = v.submerged ? 0.12f : 0.95f; // Strong muffling when inside ink

            for (u32 f = 0; f < numFrames; ++f) {
                if (v.sampleIndex >= v.totalSamples) break;

                f32 tNorm = static_cast<f32>(v.sampleIndex) / static_cast<f32>(v.totalSamples);
                f32 rawSample = 0.0f;

                switch (v.soundId) {
                case cSoundId_Shoot_Splattershot: {
                    // 440 Hz down to 140 Hz downward chirping pop
                    f32 freq = 440.0f * (1.0f - tNorm * 0.70f);
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;

                    f32 tone = std::sin(v.phase * cTwoPi);
                    // Add wet splash noise component
                    f32 noise = ((rand() % 2000) / 1000.0f - 1.0f) * 0.25f;
                    f32 env = std::pow(1.0f - tNorm, 1.8f);
                    rawSample = (tone * 0.75f + noise) * env;
                    break;
                }

                case cSoundId_Hit_Confirm: {
                    // Iconic metallic chime: 1320 Hz fundamental + 2640 Hz octave
                    v.phase += 1320.0f / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;

                    f32 tone1 = std::sin(v.phase * cTwoPi);
                    f32 tone2 = std::sin(v.phase * 2.0f * cTwoPi) * 0.5f;
                    f32 env = std::exp(-tNorm * 12.0f); // Quick exponential decay
                    rawSample = (tone1 + tone2) * env * 0.9f;
                    break;
                }

                case cSoundId_Squid_Dive: {
                    // Resonant splash downward squelch: 240 Hz down to 60 Hz
                    f32 freq = 240.0f * (1.0f - tNorm * 0.75f);
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;

                    f32 tone = std::sin(v.phase * cTwoPi);
                    f32 bubble = std::sin(v.phase * 3.5f * cTwoPi) * 0.35f;
                    f32 env = (1.0f - tNorm);
                    rawSample = (tone + bubble) * env * 0.8f;
                    break;
                }

                case cSoundId_Squid_Swim: {
                    // Soft watery bubbling
                    f32 freq = 160.0f + std::sin(tNorm * cTwoPi * 4.0f) * 40.0f;
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;
                    f32 tone = std::sin(v.phase * cTwoPi);
                    f32 env = std::sin(tNorm * 3.14159f) * 0.4f;
                    rawSample = tone * env;
                    break;
                }

                case cSoundId_Splat_Bomb_Throw: {
                    // Whoosh swoosh: 220Hz up to 600Hz and down
                    f32 pitchEnv = std::sin(tNorm * 3.14159f);
                    f32 freq = 200.0f + pitchEnv * 400.0f;
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;
                    f32 noise = ((rand() % 2000) / 1000.0f - 1.0f) * 0.5f;
                    f32 tone = std::sin(v.phase * cTwoPi) * 0.5f;
                    rawSample = (tone + noise) * pitchEnv * 0.6f;
                    break;
                }

                case cSoundId_Splat_Bomb_Explode: {
                    // Sub bass 55Hz + massive splash noise burst
                    f32 subFreq = 55.0f * (1.0f - tNorm * 0.5f);
                    v.phase += subFreq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;

                    f32 sub = std::sin(v.phase * cTwoPi);
                    f32 splash = ((rand() % 2000) / 1000.0f - 1.0f);
                    f32 envSub = std::exp(-tNorm * 6.0f);
                    f32 envSplash = std::exp(-tNorm * 8.0f);
                    rawSample = (sub * 0.8f * envSub) + (splash * 0.6f * envSplash);
                    break;
                }

                case cSoundId_Crate_Break: {
                    // Crunchy wood break: random impulse bursts + quick snap
                    f32 snap = ((rand() % 2000) / 1000.0f - 1.0f);
                    f32 env = std::pow(1.0f - tNorm, 3.0f);
                    rawSample = snap * env * 0.85f;
                    break;
                }

                case cSoundId_Low_Ink_Warning: {
                    // Dual click buzz: 780 Hz
                    v.phase += 780.0f / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;
                    f32 tone = (std::sin(v.phase * cTwoPi) > 0.0f ? 0.5f : -0.5f);
                    // Two pulses
                    f32 pulse = (tNorm < 0.45f || (tNorm > 0.55f && tNorm < 0.95f)) ? 1.0f : 0.0f;
                    rawSample = tone * pulse * 0.5f;
                    break;
                }

                case cSoundId_Roller_Fling: {
                    // Heavy air swoosh: 110Hz rising to 320Hz and down + splash
                    f32 pitchEnv = std::sin(tNorm * 3.14159f);
                    f32 freq = 110.0f + pitchEnv * 220.0f;
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;
                    f32 tone = std::sin(v.phase * cTwoPi);
                    f32 noise = ((rand() % 2000) / 1000.0f - 1.0f) * 0.45f;
                    rawSample = (tone * 0.6f + noise) * pitchEnv * 0.8f;
                    break;
                }

                case cSoundId_Charger_Charge: {
                    // Continuous rising charging whine: 350Hz up to 900Hz
                    f32 freq = 350.0f + tNorm * 550.0f;
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;
                    rawSample = std::sin(v.phase * cTwoPi) * 0.45f;
                    break;
                }

                case cSoundId_Charger_Fire: {
                    // Sharp high-velocity beam crack: 950Hz chirp down to 220Hz
                    f32 freq = 950.0f * (1.0f - tNorm * 0.75f);
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;
                    f32 laser = std::sin(v.phase * cTwoPi);
                    f32 crack = ((rand() % 2000) / 1000.0f - 1.0f) * 0.4f;
                    f32 env = std::exp(-tNorm * 9.0f);
                    rawSample = (laser + crack) * env * 0.9f;
                    break;
                }

                case cSoundId_Octoling_Splat: {
                    // Splat burst: ascending pop (350Hz -> 520Hz) + wide ink splash
                    f32 freq = 350.0f + tNorm * 170.0f;
                    v.phase += freq / static_cast<f32>(cSampleRate);
                    if (v.phase > 1.0f) v.phase -= 1.0f;
                    f32 pop = std::sin(v.phase * cTwoPi);
                    f32 splash = ((rand() % 2000) / 1000.0f - 1.0f);
                    f32 envPop = std::exp(-tNorm * 7.0f);
                    f32 envSplash = std::exp(-tNorm * 5.0f);
                    rawSample = (pop * 0.7f * envPop) + (splash * 0.6f * envSplash);
                    break;
                }

                default:
                    rawSample = 0.0f;
                    break;
                }

                // Apply lowpass filtering
                v.lpfState[0] += alphaLpf * (rawSample - v.lpfState[0]);
                v.lpfState[1] += alphaLpf * (rawSample - v.lpfState[1]);

                mixL[f] += v.lpfState[0] * leftGain;
                mixR[f] += v.lpfState[1] * rightGain;

                v.sampleIndex++;
            }

            if (v.sampleIndex >= v.totalSamples) {
                it = mVoices.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Convert mixed 32-bit float to 16-bit interleaved PCM with soft-clipping
    for (u32 f = 0; f < numFrames; ++f) {
        f32 l = mixL[f];
        f32 r = mixR[f];

        // Soft clipper: tanh approx
        f32 lClip = l / (1.0f + std::abs(l));
        f32 rClip = r / (1.0f + std::abs(r));

        outBuffer[f * 2 + 0] = static_cast<s16>(lClip * 30000.0f);
        outBuffer[f * 2 + 1] = static_cast<s16>(rClip * 30000.0f);
    }
}

void PcAudioDriver::audioThreadLoop() {
#ifdef _WIN32
    static constexpr u32 cNumBuffers = 3;
    WAVEHDR headers[cNumBuffers];
    std::vector<s16> bufferData[cNumBuffers];

    HWAVEOUT hWave = static_cast<HWAVEOUT>(mWaveOutHandle);
    if (!hWave) return;

    for (u32 i = 0; i < cNumBuffers; ++i) {
        bufferData[i].resize(cBufferSize * 2, 0);
        ZeroMemory(&headers[i], sizeof(WAVEHDR));
        headers[i].lpData = reinterpret_cast<LPSTR>(bufferData[i].data());
        headers[i].dwBufferLength = cBufferSize * 2 * sizeof(s16);
        headers[i].dwFlags = 0;
        waveOutPrepareHeader(hWave, &headers[i], sizeof(WAVEHDR));
    }

    u32 currentBuf = 0;

    while (mRunning.load()) {
        WAVEHDR& hdr = headers[currentBuf];

        // Wait until buffer is finished playing if previously queued
        if ((hdr.dwFlags & WHDR_DONE) || !(hdr.dwFlags & WHDR_PREPARED)) {
            // Fill audio buffer
            generateAudioBlock(bufferData[currentBuf].data(), cBufferSize);
            hdr.dwFlags &= ~WHDR_DONE;
            waveOutWrite(hWave, &hdr, sizeof(WAVEHDR));
            currentBuf = (currentBuf + 1) % cNumBuffers;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    for (u32 i = 0; i < cNumBuffers; ++i) {
        waveOutUnprepareHeader(hWave, &headers[i], sizeof(WAVEHDR));
    }
#endif
}

} // namespace Game
