#include "Game/Net/PlayerClone.h"
#include <cstring>

namespace Game {

PlayerClone::PlayerClone()
    : mPlayerId(0),
      mTeamId(0),
      mIsLocal(false) {
    std::memset(&mTransform, 0, sizeof(mTransform));
    std::memset(&mMotion, 0, sizeof(mMotion));
    std::memset(&mStatus, 0, sizeof(mStatus));
    std::memset(&mAim, 0, sizeof(mAim));
    std::memset(&mAction, 0, sizeof(mAction));
    std::memset(&mSpecial, 0, sizeof(mSpecial));
    std::memset(&mGear, 0, sizeof(mGear));
}

PlayerClone::~PlayerClone() {
}

void PlayerClone::init() {
    GambitActor::init();
    mPlayerId = 0;
    mTeamId = 0;
    mIsLocal = false;
    std::memset(&mTransform, 0, sizeof(mTransform));
    std::memset(&mMotion, 0, sizeof(mMotion));
    std::memset(&mStatus, 0, sizeof(mStatus));
    std::memset(&mAim, 0, sizeof(mAim));
    std::memset(&mAction, 0, sizeof(mAction));
    std::memset(&mSpecial, 0, sizeof(mSpecial));
    std::memset(&mGear, 0, sizeof(mGear));

    mStatus.health = 100.0f;
    mStatus.inkLevel = 100.0f;
}

void PlayerClone::setupClone(u32 playerId, u32 teamId, bool isLocalPlayer) {
    mPlayerId = playerId;
    mTeamId = teamId;
    mIsLocal = isLocalPlayer;
    mStatus.health = 100.0f;
    mStatus.inkLevel = 100.0f;
}

void PlayerClone::setTransform(const sead::Vector3f& pos, f32 rotY) {
    mTransform.position = pos;
    mTransform.rotationY = rotY;
}

void PlayerClone::setMotion(const sead::Vector3f& vel, bool isSquid, bool isGrounded) {
    mMotion.velocity = vel;
    mMotion.isSquid = isSquid;
    mMotion.isGrounded = isGrounded;
}

void PlayerClone::setStatus(f32 hp, f32 ink) {
    mStatus.health = hp;
    mStatus.inkLevel = ink;
}

void PlayerClone::setAim(f32 pitch, f32 yaw, f32 charge) {
    mAim.pitch = pitch;
    mAim.yaw = yaw;
    mAim.chargeLevel = charge;
}

void PlayerClone::setAction(bool firing, bool throwingSub, u32 seq) {
    mAction.isFiring = firing;
    mAction.isThrowingSub = throwingSub;
    mAction.sequenceId = seq;
}

void PlayerClone::setSpecial(f32 points, bool active) {
    mSpecial.specialPoints = points;
    mSpecial.isSpecialActive = active;
}

void PlayerClone::setGear(u32 head, u32 clothes, u32 shoes) {
    mGear.headId = head;
    mGear.clothesId = clothes;
    mGear.shoesId = shoes;
}

// Matches Game__PlayerClone channels 0..9 serialization
void PlayerClone::packChannels(u8* outBuffer, size_t maxBufferSize, size_t* outWrittenBytes) const {
    size_t requiredSize = sizeof(mPlayerId) + sizeof(mTeamId) +
                          sizeof(mTransform) + sizeof(mMotion) +
                          sizeof(mStatus) + sizeof(mAim) +
                          sizeof(mAction) + sizeof(mSpecial) + sizeof(mGear);

    if (maxBufferSize < requiredSize || !outBuffer) {
        if (outWrittenBytes) *outWrittenBytes = 0;
        return;
    }

    u8* ptr = outBuffer;
    std::memcpy(ptr, &mPlayerId, sizeof(mPlayerId)); ptr += sizeof(mPlayerId);
    std::memcpy(ptr, &mTeamId, sizeof(mTeamId));     ptr += sizeof(mTeamId);
    std::memcpy(ptr, &mTransform, sizeof(mTransform)); ptr += sizeof(mTransform);
    std::memcpy(ptr, &mMotion, sizeof(mMotion));       ptr += sizeof(mMotion);
    std::memcpy(ptr, &mStatus, sizeof(mStatus));       ptr += sizeof(mStatus);
    std::memcpy(ptr, &mAim, sizeof(mAim));             ptr += sizeof(mAim);
    std::memcpy(ptr, &mAction, sizeof(mAction));       ptr += sizeof(mAction);
    std::memcpy(ptr, &mSpecial, sizeof(mSpecial));     ptr += sizeof(mSpecial);
    std::memcpy(ptr, &mGear, sizeof(mGear));           ptr += sizeof(mGear);

    if (outWrittenBytes) *outWrittenBytes = requiredSize;
}

bool PlayerClone::unpackChannels(const u8* inBuffer, size_t bufferSize) {
    size_t requiredSize = sizeof(mPlayerId) + sizeof(mTeamId) +
                          sizeof(mTransform) + sizeof(mMotion) +
                          sizeof(mStatus) + sizeof(mAim) +
                          sizeof(mAction) + sizeof(mSpecial) + sizeof(mGear);

    if (bufferSize < requiredSize || !inBuffer) {
        return false;
    }

    const u8* ptr = inBuffer;
    std::memcpy(&mPlayerId, ptr, sizeof(mPlayerId)); ptr += sizeof(mPlayerId);
    std::memcpy(&mTeamId, ptr, sizeof(mTeamId));     ptr += sizeof(mTeamId);
    std::memcpy(&mTransform, ptr, sizeof(mTransform)); ptr += sizeof(mTransform);
    std::memcpy(&mMotion, ptr, sizeof(mMotion));       ptr += sizeof(mMotion);
    std::memcpy(&mStatus, ptr, sizeof(mStatus));       ptr += sizeof(mStatus);
    std::memcpy(&mAim, ptr, sizeof(mAim));             ptr += sizeof(mAim);
    std::memcpy(&mAction, ptr, sizeof(mAction));       ptr += sizeof(mAction);
    std::memcpy(&mSpecial, ptr, sizeof(mSpecial));     ptr += sizeof(mSpecial);
    std::memcpy(&mGear, ptr, sizeof(mGear));           ptr += sizeof(mGear);

    return true;
}

void PlayerClone::update() {
    if (!mIsLocal) {
        // Remote clone interpolation
        mTransform.position.x += mMotion.velocity.x;
        mTransform.position.y += mMotion.velocity.y;
        mTransform.position.z += mMotion.velocity.z;
    }
}

void PlayerClone::draw() {
    GambitActor::draw();
}

} // namespace Game
