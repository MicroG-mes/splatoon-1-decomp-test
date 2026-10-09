#include "Game/MapObj/Obj_Windsock.h"
#include <cmath>

namespace Game {

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Obj_Windsock::Obj_Windsock()
{
}

void Obj_Windsock::init() {
    mTimer = 0;
    setWind(mWind);
}

void Obj_Windsock::setWind(const sead::Vector3f& wind) {
    mWind = wind;
    float horizSpeed = std::sqrt(wind.x * wind.x + wind.z * wind.z);
    if (horizSpeed > 0.01f) {
        mYaw = std::atan2(wind.x, wind.z);
    }
    mInflation = horizSpeed / mParams.mMaxWindSpeed;
    if (mInflation > 1.0f) mInflation = 1.0f;
    // Droop angle blends from max droop to near-horizontal based on inflation
    mPitchAngle = mParams.mMaxDroopAngle * (1.0f - mInflation);
}

void Obj_Windsock::update() {
    mTimer++;
    // Dynamic cloth ripple / flutter
    float horizSpeed = std::sqrt(mWind.x * mWind.x + mWind.z * mWind.z);
    float flutterSpeed = horizSpeed * mParams.mFlutterFrequency;
    float flutter = std::sin(static_cast<float>(mTimer) * flutterSpeed) * 0.05f * mInflation;

    mPitchAngle = (mParams.mMaxDroopAngle * (1.0f - mInflation)) + flutter;
}

} // namespace Game
