#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

enum class MiniGameType : u32 {
    cSquidJump  = 0, // Squid Jump (Ika Jump) - 8-bit vertical platformer
    cSquidBall  = 1, // Squid Ball (Ika Volley) - Amiibo unlocked
    cSquidRacer = 2, // Squid Racer (Ika Race) - Amiibo unlocked
    cSquidBeatz = 3  // Squid Beatz (Ika Radio) - Amiibo rhythm game
};

/**
 * Obj_PlazaGame (Squid Jump Arcade Cabinet in Inkopolis Plaza)
 * Address: vtable @ 0x100D326C
 * Authentic path: D:/home/Cafe/Gambit/App/Program/Game/Plaza/Obj_PlazaGame.cpp
 *
 * Real PowerPC methods from Gambit.elf:
 *   vfunc_3  @ 0x0259CF20 (size 56): Cabinet placement and bound registration
 *   vfunc_7  @ 0x0259CF58 (size 364): Attract mode CRT screen cycling
 *   vfunc_11 @ 0x0259D1FC (size 68): Phosphor scanline intensity
 *   vfunc_47 @ 0x0259DAA0 (size 64): Player interaction trigger ('A' button)
 *   vfunc_1  @ 0x0259D934 (size 320): Destructor & actor cleanup
 */
class Obj_PlazaGame : public GambitActor {
public:
    Obj_PlazaGame();
    virtual ~Obj_PlazaGame() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    // Authentic PPC vfuncs
    virtual void vfunc_3();
    virtual void vfunc_7();
    virtual f32 vfunc_11();
    virtual bool vfunc_47();

    void setup(const sead::Vector3f& cabinetPos);
    bool checkPlayerProximity(const sead::Vector3f& playerPos, f32 interactRadius = 2.5f);
    void updateProximity(const sead::Vector3f& playerPos, f32 interactRadius = 2.5f);
    void triggerGame();
    bool launchGame(MiniGameType game);
    void exitGame();

    void submitHighScore(MiniGameType game, u32 score);
    u32 getHighScore(MiniGameType game) const;

    bool isInteractionActive() const { return mIsPlayerNearby; }
    bool isGameActive() const { return mIsPlaying; }
    bool isPlaying() const { return mIsPlaying; }
    MiniGameType getActiveGame() const { return mActiveGame; }
    bool isNearby() const { return mIsPlayerNearby; }
    f32 getScreenGlow() const { return mScreenGlow; }
    const sead::Vector3f& getPosition() const { return mPosition; }

protected:
    sead::Vector3f mPosition;
    f32 mInteractRadius;
    f32 mScreenGlow;
    s32 mAttractFrame;
    bool mIsPlayerNearby;
    bool mIsPlaying;
    MiniGameType mActiveGame;

    u32 mHighScores[4];
    bool mGameUnlocked[4];
};

} // namespace Game
