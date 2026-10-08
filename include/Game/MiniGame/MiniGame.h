#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"
#include <vector>

namespace Game {

enum class MiniGameType : u32 {
    SquidJump   = 0, // IkaJump (Default)
    SquidRacer  = 1, // IkaRacer (Unlocked via Inkling Boy Amiibo)
    Squidball   = 2, // IkaBall (Unlocked via Inkling Girl Amiibo)
    SquidBeatz  = 3  // IkaRadio (Unlocked via Inkling Squid Amiibo)
};

enum class PlatformType : u32 {
    Normal   = 0, // Solid platform
    MovingH  = 1, // Moving horizontally
    MovingV  = 2, // Moving vertically
    Sand     = 3, // Disappears shortly after landing
    Spring   = 4  // Mega trampoline boost
};

struct MiniGamePlatform {
    sead::Vector2f pos;
    sead::Vector2f size;
    PlatformType type;
    f32 movePhase;
    bool isTriggered;
    f32 timer;
};

class MiniGame : public GambitActor {
public:
    MiniGame();
    virtual ~MiniGame() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void selectGame(MiniGameType type);
    void resetGame();

    // Squid Jump gameplay controls
    void startChargeJump();
    void releaseJump();
    void steerHorizontal(f32 inputX);

    u32 getCurrentScore() const { return mScore; }
    u32 getCurrentStage() const { return mStage; }
    bool isGameOver() const { return mIsGameOver; }
    bool isStageClear() const { return mIsStageClear; }

    const sead::Vector2f& getPlayerPos() const { return mPlayerPos; }

protected:
    void updateSquidJump();
    void generatePlatformsForStage(u32 stage);
    void checkCollisions();

    MiniGameType mCurrentType;
    u32 mScore;
    u32 mStage;
    s32 mLives;

    // Squid Jump state
    sead::Vector2f mPlayerPos;
    sead::Vector2f mPlayerVel;
    bool mIsGrounded;
    bool mIsCharging;
    f32 mChargeAmount; // 0.0 to 1.0

    f32 mCameraY;
    f32 mStageGoalY;

    bool mIsGameOver;
    bool mIsStageClear;

    std::vector<MiniGamePlatform> mPlatforms;
    undefined mReserved[0x38];
};

} // namespace Game
