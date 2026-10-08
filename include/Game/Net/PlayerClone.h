#pragma once

#include "types.h"
#include "Game/Actor/GambitActor.h"
#include "sead/math/seadVector.h"

namespace Game {

// Replicated channel data structs (matches Game__PlayerClone channels 0..9)
struct NetChannelTransform {
    sead::Vector3f position;
    f32 rotationY;
};

struct NetChannelMotion {
    sead::Vector3f velocity;
    bool isSquid;
    bool isGrounded;
};

struct NetChannelStatus {
    f32 health;
    f32 inkLevel;
};

struct NetChannelAim {
    f32 pitch;
    f32 yaw;
    f32 chargeLevel;
};

struct NetChannelAction {
    bool isFiring;
    bool isThrowingSub;
    u32 sequenceId;
};

struct NetChannelSpecial {
    f32 specialPoints;
    bool isSpecialActive;
};

struct NetChannelGear {
    u32 headId;
    u32 clothesId;
    u32 shoesId;
};

// Decompiled from PPC: Game::PlayerClone @ 0x02669C3C
class PlayerClone : public GambitActor {
public:
    PlayerClone();
    virtual ~PlayerClone() override;

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;

    void setupClone(u32 playerId, u32 teamId, bool isLocalPlayer);

    // Channel serializers
    void packChannels(u8* outBuffer, size_t maxBufferSize, size_t* outWrittenBytes) const;
    bool unpackChannels(const u8* inBuffer, size_t bufferSize);

    // Setters for local simulation
    void setTransform(const sead::Vector3f& pos, f32 rotY);
    void setMotion(const sead::Vector3f& vel, bool isSquid, bool isGrounded);
    void setStatus(f32 hp, f32 ink);
    void setAim(f32 pitch, f32 yaw, f32 charge);
    void setAction(bool firing, bool throwingSub, u32 seq);
    void setSpecial(f32 points, bool active);
    void setGear(u32 head, u32 clothes, u32 shoes);

    // Getters
    u32 getPlayerId() const { return mPlayerId; }
    u32 getTeamId() const { return mTeamId; }
    bool isLocal() const { return mIsLocal; }
    const NetChannelTransform& getTransform() const { return mTransform; }
    const NetChannelMotion& getMotion() const { return mMotion; }
    const NetChannelStatus& getStatus() const { return mStatus; }
    const NetChannelAim& getAim() const { return mAim; }
    const NetChannelAction& getAction() const { return mAction; }
    const NetChannelSpecial& getSpecial() const { return mSpecial; }
    const NetChannelGear& getGear() const { return mGear; }

private:
    u32 mPlayerId;
    u32 mTeamId;
    bool mIsLocal;

    NetChannelTransform mTransform;
    NetChannelMotion    mMotion;
    NetChannelStatus    mStatus;
    NetChannelAim       mAim;
    NetChannelAction    mAction;
    NetChannelSpecial   mSpecial;
    NetChannelGear      mGear;
};

} // namespace Game
