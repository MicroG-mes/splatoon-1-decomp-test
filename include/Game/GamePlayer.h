#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "Game/Actor/GambitActor.h"
#include "Game/GamePlayerBehindCamera.h"
#include "cafe/vpad.h"

namespace Game {

struct PlayerWeaponShachihoko;
struct PlayerInkAction;
struct PlayerModel;
struct PlayerEffect;
struct PlayerNetControl;

// State IDs recovered from binary strings at 0x0262FE32 - 0x02630AB6
enum class PlayerStateID : u32 {
    Human_Wait   = 0,
    Human_Move   = 1,
    Human_JumpSt = 2,
    Human_Jump   = 3,
    Human_JumpEd = 4,
    Human_Jet    = 5, // Super Jump
    Human_JetEd  = 6,
    Squid_Wait   = 7,
    Squid_Move   = 8,
    Squid_JumpSt = 9,
    Squid_Jump   = 10,
    Squid_JumpEd = 11,
    Squid_Jet    = 12,
    Squid_JetEd  = 13,
    Squid_InkRail= 14,
    Squid_Geyser = 15,
    Squid_ObjAim = 16,
};

struct PlayerStateCloneEvent {
    byte eventId;
};

class Player : public GambitActor {
public:
    Player();
    virtual ~Player() override;

    // Actor lifecycle
    virtual void init() override;
    virtual void update() override;
    virtual void postUpdate() override;
    virtual void draw() override;

    // State machine management
    void changeState(PlayerStateID stateId);
    PlayerStateID getCurrentState() const { return mCurrentState; }
    f32 getInkTankAmount() const { return mInkTankAmount; }
    bool isSquidState() const;
    bool isHumanState() const;

    // State tick & transition methods
    void enterState(PlayerStateID state);
    void updateState(PlayerStateID state);
    void exitState(PlayerStateID state);

    // Input & Physics
    void handleInput(const VPADStatus& vpad);
    void updatePhysics();
    void updateInkSwimming();
    void tryShoot();
    void trySubWeapon();
    const sead::Vector3f& getVelocity() const { return mVelocity; }
    void setVelocity(const sead::Vector3f& vel) { mVelocity = vel; }

    // Network event handlers
    void receiveDie_Net();
    void receiveAirFall_Net();
    void receiveWaterFall_Net();
    void receiveRevival_Net();
    void receiveUnk_Net();
    void receiveStartDokanWarp_Net();
    void receiveUnk2_Net();
    void receiveEndDokanWarp_Net();

    // Authentic Ghidra Decompiled vfuncs (from PlayerCtrl vtable 0x100E9700)
    virtual const char* getActorName();      // vfunc_36 (0x0265A688)
    virtual const char* getControllerName(); // vfunc_37 (0x0265A730)
    virtual void enterHumanForm();           // vfunc_52 (0x02659F7C)
    virtual void enterSquidForm();           // vfunc_53 (0x02659FD0)
    virtual void updatePhysicsStep();        // vfunc_7 (0x02641C1C)
    virtual void updateGroundSlope();        // vfunc_11 (0x0264FF04)

public:
    u8 field0_0x0[44];
    s32 teamId;           // 0x2C
    u8 field2_0x30[40];
    s32 isOtherPlayer;    // 0x58
    s32 isLocalPlayer;    // 0x5C
    s16 shotBulletsNum;   // 0x60
    u8 field7_0x62[22];
    s32 weaponId;         // 0x78
    s32 subWeaponId;      // 0x7C
    s32 specialWeaponId;  // 0x80
    u8 field11_0x84[24];
    u32 playerIndex;      // 0x9C (0 to 7)
    s32 playerId;         // 0xA0
    void* playerActorPtr; // 0xA4
    void* matchMgrPtr;    // 0xA8
    u8 field13_0xac[44];
    s16 isShooting;       // 0xD8
    s32 isSubWeaponHeld;  // 0xDC
    u8 field16_0xde[26];
    s32 useSubDelayFrm;   // 0xF8
    s16 isSwimming;       // 0xFC
    u8 field19_0xfe[126];
    sead::Vector3f mPosition;        // 0x248
    u8 field20_0x254[24];
    sead::Vector3f mCollisionOffset; // 0x26C
    u8 field21_0x278[160];
    sead::Vector3f mVelocity;        // 0x318 (0x318.x, 0x31c.y, 0x320.z)
    u8 field22_0x324[252];
    f32 mTargetVelocity;             // 0x420
    f32 mCurrentVelocity;            // 0x424
    f32 mAccelDelta;                 // 0x428
    s32 mAccelTimer;                 // 0x42C
    u8 field23_0x430[92];
    char mIsSlopeGrounded;           // 0x48C
    u8 field24_0x48D[71];
    s32 deathFrm;                    // 0x4D4
    u8 field25_0x4D8[172];
    char mIsAiming;                  // 0x584
    u8 mIsSuperSpeed;                // 0x585
    u8 field26_0x586[22];
    char mIsInInk;                   // 0x59C
    u8 field27_0x59D[19];
    s32 mSubmergedState;             // 0x5B0
    u8 field28_0x5B4[56];
    f32 mInkDepth;                   // 0x5EC
    u8 field29_0x5F0[4];
    sead::Vector3f mGroundNormal;    // 0x5F4 (0x5f4.x, 0x5f8.y, 0x5fc.z)
    u8 field30_0x600[40];
    f32 mDistanceToFloor;            // 0x628
    u8 field31_0x62C[300];
    void* mGearSkillMgr;             // 0x758
    void* mJumpController;           // 0x75C
    void* mAnimController;           // 0x760
    PlayerInkAction* playerInkAction;// 0x764
    PlayerModel* playerModel;        // 0x768
    PlayerEffect* playerEffect;      // 0x76C
    void* mSoundController;          // 0x770
    void* mFormStateMachine;         // 0x774
    void* mWeaponController;         // 0x778
    u8 field32_0x77C[8];
    PlayerNetControl* playerNetControl;// 0x784
    u8 field33_0x788[44];
    PlayerStateID mCurrentState;     // 0x7B4
    u8 field34_0x7B8;
    byte mStateSubFlag;              // 0x7B9
    u8 field35_0x7BA[214];
    f32 swimSpeed;                   // 0x890
    u8 field36_0x894[60];
    PlayerBehindCamera* playerBehindCamera; // 0x8D0

protected:
    f32 mInkTankAmount;     // 0.0 to 1.0 (empty to full)
    bool mIsGrounded;
    bool mIsInFriendlyInk;
    bool mIsInEnemyInk;
};

class PlayerCloneHandle {
public:
    static void unpackStateEvent(Player* player, PlayerStateCloneEvent* event, u32* unk);
};

} // namespace Game
