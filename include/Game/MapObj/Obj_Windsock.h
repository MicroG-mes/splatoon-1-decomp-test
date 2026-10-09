#pragma once

#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <string>

namespace Game {

class Obj_Windsock : public GambitActor {
public:
    struct Params {
        float mMaxWindSpeed = 10.0f;     // Speed for full horizontal inflation
        float mMaxDroopAngle = -1.05f;   // Droop when calm (~-60 deg in rad)
        float mFlutterFrequency = 0.15f; // Cloth flutter speed
    };

    Obj_Windsock();
    virtual ~Obj_Windsock() = default;

    virtual void init() override;
    virtual void update() override;

    const Params& getParams() const { return mParams; }
    const std::string& getName() const { return mName; }

    void setWind(const sead::Vector3f& wind);
    const sead::Vector3f& getWind() const { return mWind; }

    float getYaw() const { return mYaw; }
    float getInflation() const { return mInflation; }
    float getPitchAngle() const { return mPitchAngle; }

private:
    std::string mName = "Obj_Windsock";
    Params mParams;

    sead::Vector3f mWind = sead::Vector3f(3.0f, 0.0f, 0.0f);
    float mYaw = 0.0f;
    float mPitchAngle = 0.0f;
    float mInflation = 0.3f;
    int mTimer = 0;
};

} // namespace Game
