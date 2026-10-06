#include "Game/Actor/GambitActor.h"

namespace Game {

GambitActor::GambitActor()
    : mPosition(0.0f, 0.0f, 0.0f),
      mRotation(0.0f, 0.0f, 0.0f),
      mScale(1.0f, 1.0f, 1.0f),
      mChildHeap(nullptr),
      mActorId(0),
      mActorFlags(0) {}

GambitActor::~GambitActor() {
    destroy();
}

void GambitActor::init() {}
void GambitActor::update() {}
void GambitActor::postUpdate() {}
void GambitActor::draw() {}
void GambitActor::destroy() {}

sead::Heap* GambitActor::createActorLoadChildHeap(size_t heapSize, const char* heapName) {
    return nullptr;
}

void GambitActor::openHIO() {}
void GambitActor::moveToActor() {}
void GambitActor::onDeathOrComplete() {}

} // namespace Game
