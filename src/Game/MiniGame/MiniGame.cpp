#include "Game/MiniGame/MiniGame.h"

namespace Game {

MiniGame::MiniGame()
    : mCurrentType(MiniGameType::SquidJump),
      mScore(0),
      mStage(1),
      mLives(3),
      mPlayerPos(0.0f, 0.0f),
      mPlayerVel(0.0f, 0.0f),
      mIsGrounded(true),
      mIsCharging(false),
      mChargeAmount(0.0f),
      mCameraY(0.0f),
      mStageGoalY(1000.0f),
      mIsGameOver(false),
      mIsStageClear(false) {
}

MiniGame::~MiniGame() {
}

void MiniGame::init() {
    GambitActor::init();
    selectGame(mCurrentType);
}

void MiniGame::selectGame(MiniGameType type) {
    mCurrentType = type;
    resetGame();
}

void MiniGame::resetGame() {
    mScore = 0;
    mStage = 1;
    mLives = 3;
    mPlayerPos = sead::Vector2f(160.0f, 20.0f);
    mPlayerVel = sead::Vector2f(0.0f, 0.0f);
    mIsGrounded = true;
    mIsCharging = false;
    mChargeAmount = 0.0f;
    mCameraY = 0.0f;
    mIsGameOver = false;
    mIsStageClear = false;

    generatePlatformsForStage(mStage);
}

void MiniGame::generatePlatformsForStage(u32 stage) {
    mPlatforms.clear();

    // Floor platform
    MiniGamePlatform floor = { sead::Vector2f(160.0f, 0.0f), sead::Vector2f(320.0f, 16.0f), PlatformType::Normal, 0.0f, false, 0.0f };
    mPlatforms.push_back(floor);

    // Procedural platforms ascending upwards
    f32 y = 80.0f;
    mStageGoalY = 1000.0f + stage * 200.0f;

    while (y < mStageGoalY) {
        f32 x = 40.0f + static_cast<f32>((static_cast<u32>(y * 17) % 240));
        PlatformType ptype = PlatformType::Normal;

        if (static_cast<u32>(y) % 5 == 0) {
            ptype = PlatformType::MovingH;
        } else if (static_cast<u32>(y) % 7 == 0) {
            ptype = PlatformType::Spring;
        } else if (static_cast<u32>(y) % 11 == 0) {
            ptype = PlatformType::Sand;
        }

        MiniGamePlatform plat = { sead::Vector2f(x, y), sead::Vector2f(48.0f, 12.0f), ptype, 0.0f, false, 0.0f };
        mPlatforms.push_back(plat);

        y += 60.0f + static_cast<f32>(stage * 2);
    }

    // Goal platform
    MiniGamePlatform goal = { sead::Vector2f(160.0f, mStageGoalY), sead::Vector2f(80.0f, 16.0f), PlatformType::Normal, 0.0f, false, 0.0f };
    mPlatforms.push_back(goal);
}

void MiniGame::startChargeJump() {
    if (mIsGrounded && !mIsCharging) {
        mIsCharging = true;
        mChargeAmount = 0.0f;
    }
}

void MiniGame::releaseJump() {
    if (mIsCharging && mIsGrounded) {
        f32 jumpPower = 6.0f + mChargeAmount * 12.0f; // Scale up with charge
        mPlayerVel.y = jumpPower;
        mIsGrounded = false;
        mIsCharging = false;
        mChargeAmount = 0.0f;
    }
}

void MiniGame::steerHorizontal(f32 inputX) {
    if (!mIsGrounded) {
        mPlayerVel.x = inputX * 4.0f;
    }
}

void MiniGame::checkCollisions() {
    // Only collide when moving downwards
    if (mPlayerVel.y <= 0.0f) {
        for (auto& plat : mPlatforms) {
            f32 halfW = plat.size.x * 0.5f;
            f32 halfH = plat.size.y * 0.5f;

            if (mPlayerPos.x >= plat.pos.x - halfW && mPlayerPos.x <= plat.pos.x + halfW &&
                mPlayerPos.y >= plat.pos.y - halfH && mPlayerPos.y <= plat.pos.y + halfH + 8.0f) {

                if (plat.type == PlatformType::Spring) {
                    mPlayerVel.y = 22.0f; // Trampoline super jump!
                    mIsGrounded = false;
                    mScore += 100;
                } else {
                    mPlayerPos.y = plat.pos.y + halfH;
                    mPlayerVel.y = 0.0f;
                    mIsGrounded = true;

                    if (plat.type == PlatformType::Sand) {
                        plat.isTriggered = true;
                    }
                }
                break;
            }
        }
    }
}

void MiniGame::updateSquidJump() {
    if (mIsGameOver || mIsStageClear) {
        return;
    }

    if (mIsCharging) {
        mChargeAmount += 0.04f;
        if (mChargeAmount > 1.0f) {
            mChargeAmount = 1.0f;
        }
    }

    // Apply gravity
    if (!mIsGrounded) {
        mPlayerVel.y -= 0.45f;
        mPlayerPos.x += mPlayerVel.x;
        mPlayerPos.y += mPlayerVel.y;

        // Wrap around screen boundaries horizontally
        if (mPlayerPos.x < 0.0f) {
            mPlayerPos.x += 320.0f;
        } else if (mPlayerPos.x > 320.0f) {
            mPlayerPos.x -= 320.0f;
        }

        checkCollisions();
    }

    // Update moving platforms
    for (auto& plat : mPlatforms) {
        if (plat.type == PlatformType::MovingH) {
            plat.movePhase += 0.03f;
            plat.pos.x = 160.0f + 80.0f * __builtin_sinf(plat.movePhase);
        } else if (plat.type == PlatformType::Sand && plat.isTriggered) {
            plat.timer += 0.016f;
            if (plat.timer > 0.5f) {
                plat.pos.y = -999.0f; // Collapsed
            }
        }
    }

    // Camera scrolling follows player
    if (mPlayerPos.y - mCameraY > 160.0f) {
        mCameraY = mPlayerPos.y - 160.0f;
    }

    // Score based on altitude
    u32 currentAltitude = static_cast<u32>(mPlayerPos.y * 0.1f);
    if (currentAltitude > mScore) {
        mScore = currentAltitude;
    }

    // Check stage clear
    if (mPlayerPos.y >= mStageGoalY) {
        mIsStageClear = true;
        mScore += 1000 * mStage;
    }

    // Check falling below screen
    if (mPlayerPos.y < mCameraY - 50.0f) {
        mLives--;
        if (mLives <= 0) {
            mIsGameOver = true;
        } else {
            // Respawn on closest platform or floor
            mPlayerPos = sead::Vector2f(160.0f, mCameraY + 20.0f);
            mPlayerVel = sead::Vector2f(0.0f, 0.0f);
            mIsGrounded = true;
        }
    }
}

void MiniGame::update() {
    switch (mCurrentType) {
        case MiniGameType::SquidJump:
            updateSquidJump();
            break;
        default:
            break;
    }
}

void MiniGame::draw() {
    GambitActor::draw();
}

} // namespace Game
