#include "Game/Enemy/RailKingSchedule.h"
#include "Game/Map/BymlParser.h"
#include <fstream>
#include <algorithm>

namespace Game {

RailKingScheduleMgr::RailKingScheduleMgr()
    : mMaxPattern(0) {
}

RailKingScheduleMgr::~RailKingScheduleMgr() {}

bool RailKingScheduleMgr::loadFromByml(const char* bymlPath) {
    if (!bymlPath) return false;

    std::ifstream file(bymlPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<u8> buffer(static_cast<size_t>(fileSize));
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        return false;
    }

    BymlParser parser;
    if (!parser.parse(buffer.data(), buffer.size()) || !parser.getRoot() || !parser.getRoot()->isArray()) {
        return false;
    }

    mEvents.clear();
    mMaxPattern = 0;
    const auto* root = parser.getRoot();
    size_t count = root->getArraySize();

    for (size_t i = 0; i < count; ++i) {
        const auto* elem = root->getElement(i);
        if (!elem || !elem->isDictionary()) continue;

        RailKingScheduleEvent evt;
        evt.frame = static_cast<u32>(elem->getInt("Frame", 0));
        evt.pattern = static_cast<u32>(elem->getInt("Pattern", 0));
        evt.bullet = (elem->getInt("Bullet", 0) != 0);
        evt.rallyPunch = (elem->getInt("RallyPunch", 0) != 0);
        evt.punchL = (elem->getInt("PunchL", 0) != 0);
        evt.punchR = (elem->getInt("PunchR", 0) != 0);
        evt.bomb = (elem->getInt("Bomb", 0) != 0);

        if (evt.pattern > mMaxPattern) {
            mMaxPattern = evt.pattern;
        }

        mEvents.push_back(evt);
    }

    return !mEvents.empty();
}

const RailKingScheduleEvent* RailKingScheduleMgr::getEvent(size_t index) const {
    if (index < mEvents.size()) {
        return &mEvents[index];
    }
    return nullptr;
}

std::vector<const RailKingScheduleEvent*> RailKingScheduleMgr::getEventsForPattern(u32 pattern) const {
    std::vector<const RailKingScheduleEvent*> result;
    for (const auto& e : mEvents) {
        if (e.pattern == pattern) {
            result.push_back(&e);
        }
    }
    return result;
}

const RailKingScheduleEvent* RailKingScheduleMgr::getEventAtFrame(u32 pattern, u32 frame) const {
    for (const auto& e : mEvents) {
        if (e.pattern == pattern && e.frame == frame) {
            return &e;
        }
    }
    return nullptr;
}

const RailKingScheduleEvent* RailKingScheduleMgr::getNextEvent(u32 pattern, u32 currentFrame) const {
    const RailKingScheduleEvent* next = nullptr;
    for (const auto& e : mEvents) {
        if (e.pattern == pattern && e.frame > currentFrame) {
            if (!next || e.frame < next->frame) {
                next = &e;
            }
        }
    }
    return next;
}

} // namespace Game
