#pragma once

#include "types.h"
#include <vector>

namespace Game {

struct RailKingScheduleEvent {
    u32 frame;
    u32 pattern;
    bool bullet;
    bool rallyPunch;
    bool punchL;
    bool punchR;
    bool bomb;
};

/**
 * RailKingScheduleMgr
 * Authentic 72-event DJ Octavio (Enm_RailKing) boss battle timeline & attack scheduler
 * parsed directly from content/Static/RailKingSchedule.byaml.
 */
class RailKingScheduleMgr {
public:
    RailKingScheduleMgr();
    ~RailKingScheduleMgr();

    bool loadFromByml(const char* bymlPath);

    size_t getTotalEventCount() const { return mEvents.size(); }
    const RailKingScheduleEvent* getEvent(size_t index) const;
    std::vector<const RailKingScheduleEvent*> getEventsForPattern(u32 pattern) const;
    const RailKingScheduleEvent* getEventAtFrame(u32 pattern, u32 frame) const;
    const RailKingScheduleEvent* getNextEvent(u32 pattern, u32 currentFrame) const;

    u32 getMaxPattern() const { return mMaxPattern; }

private:
    std::vector<RailKingScheduleEvent> mEvents;
    u32 mMaxPattern;
};

} // namespace Game
