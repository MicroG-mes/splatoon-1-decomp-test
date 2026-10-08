#pragma once

#include "types.h"
#include "cafe/vpad.h"

namespace Game {

struct PcRawInputState {
    bool keyW;
    bool keyA;
    bool keyS;
    bool keyD;
    bool keySpace;
    bool keyShift;
    bool keyE;
    bool keyR;
    bool keyTab;
    bool mouseLeft;
    bool mouseRight;
    f32 mouseDeltaX;
    f32 mouseDeltaY;
    f32 mouseSensitivity;

    PcRawInputState()
        : keyW(false), keyA(false), keyS(false), keyD(false)
        , keySpace(false), keyShift(false), keyE(false), keyR(false), keyTab(false)
        , mouseLeft(false), mouseRight(false)
        , mouseDeltaX(0.0f), mouseDeltaY(0.0f)
        , mouseSensitivity(0.02f)
    {}
};

class PcInputBridge {
public:
    PcInputBridge();
    ~PcInputBridge();

    void init();
    void reset();

    // Translates PC mouse & keyboard states into standard Wii U VPADStatus
    void update(const PcRawInputState& raw, VPADStatus* outStatus = nullptr);

    const VPADStatus& getStatus() const { return mCurrentStatus; }
    u32 getHold() const { return mCurrentStatus.hold; }
    u32 getTrigger() const { return mCurrentStatus.trigger; }
    u32 getRelease() const { return mCurrentStatus.release; }

private:
    VPADStatus mCurrentStatus;
    u32 mPrevHold;
};

} // namespace Game
