#pragma once

#include "types.h"
#include "sead/math/seadVector.h"
#include "Game/GamePlayerBehindCamera.h"

namespace Game {

struct PlayerWeaponShachihoko;
struct PlayerInkAction;
struct PlayerModel;
struct PlayerEffect;
struct PlayerNetControl;

struct PlayerStateCloneEvent {
    byte eventId;
};

class Player {
public:
    undefined field0_0x0[44];
    s32 teamId;
    undefined field2_0x30[36];
    u32 field3_0x54;
    s32 isLocalPlayer;
    undefined field5_0x5c[6];
    s16 shotBulletsNum;
    undefined field7_0x64[20];
    s32 weaponId;
    s32 subWeaponId;
    s32 specialWeaponid;
    undefined field11_0x84[24];
    s32 playerId;
    undefined field13_0xa0[56];
    s16 isShooting;
    s32 isSubWeaponHeld;
    undefined field16_0xde[26];
    s32 useSubDelayFrm;
    s16 isSwimming;
    undefined field19_0xfe[190];
    sead::Vector3f* shotDir;
    undefined field21_0x1c0[8];
    f32* nextShotDir;
    undefined field23_0x1cc[44];
    PlayerWeaponShachihoko* playerWeaponShachihoko;
    undefined field25_0x1fc[560];
    s32 field26_0x42c;
    undefined field27_0x430[164];
    s32 field28_0x4d4;
    s32 field29_0x4d8;
    s32 field30_0x4dc;
    s32 deathFrm;
    undefined field32_0x4e4[18];
    undefined1 killAllEffect;
    undefined field34_0x4f7[209];
    s32 jumpframe;
    undefined field36_0x5cc[296];
    s32 field37_0x6f4;
    undefined field38_0x6f8[16];
    char field39_0x708;
    undefined field40_0x709[87];
    PlayerInkAction* playerInkAction;
    undefined1 playerCollision;
    undefined field43_0x765[3];
    PlayerModel* playerModel;
    undefined field45_0x76c[4];
    PlayerEffect* playerEffect;
    undefined field47_0x774[16];
    PlayerNetControl* playerNetControl;
    undefined field49_0x788[44];
    s32 demoPlaceType;
    undefined field51_0x7b8;
    byte field52_0x7b9;
    undefined field53_0x7ba[62];
    s32 turfPaint;
    undefined field55_0x7fc[12];
    u32 field56_0x808;
    undefined field57_0x80c[128];
    f32 swimSpeed;
    undefined field59_0x890[64];
    PlayerBehindCamera* playerBehindCamera;

    virtual ~Player();

    void receiveDie_Net();
    void receiveAirFall_Net();
    void receiveWaterFall_Net();
    void receiveRevival_Net();
    void receiveUnk_Net();
    void receiveStartDokanWarp_Net();
    void receiveUnk2_Net();
    void receiveEndDokanWarp_Net();
};

class PlayerCloneHandle {
public:
    static void unpackStateEvent(Player* player, PlayerStateCloneEvent* event, u32* unk);
};

} // namespace Game
