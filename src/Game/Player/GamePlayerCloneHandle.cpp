#include "Game/GamePlayer.h"

namespace Game {

void PlayerCloneHandle::unpackStateEvent(Player* player, PlayerStateCloneEvent* event, u32* unk) {
    if (!player || !event) {
        return;
    }

    const byte eventId = event->eventId;

    switch (eventId) {
        case 0:
            player->receiveDie_Net();
            break;
        case 1:
            player->receiveAirFall_Net();
            break;
        case 2:
            player->receiveWaterFall_Net();
            break;
        case 3:
            player->receiveRevival_Net();
            break;
        case 4:
            player->receiveUnk_Net();
            break;
        case 5:
            player->receiveStartDokanWarp_Net();
            break;
        case 6:
            player->receiveUnk2_Net();
            break;
        case 7:
            player->receiveEndDokanWarp_Net();
            break;
        default:
            break;
    }
}

} // namespace Game
